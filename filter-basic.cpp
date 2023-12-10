#include <cstdio>  // 画像を変更したいときはlaunch.jsonからargs[./data/！ここ変えて！.pgm]
#include <cstdlib>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Input image is required.\n");
    return EXIT_FAILURE;
  }

  // 画像の読み込み
  cv::Mat img = cv::imread(argv[1], cv::IMREAD_ANYCOLOR);
  if (img.empty()) {  // 画像ファイルが見つからない場合の処理
    printf("Input image is not found.\n");
    return EXIT_FAILURE;
  }

  cv::Mat img_out;
  img_out = img.clone();

  const int N = 3;  // ここでフィルターの平均サイズ(ぼける粗さ)
  const int N2 = N / 2;

  const int W = img.cols;
  const int H = img.rows;

  uint8_t *in, *out;
  in = img.data;
  out = img_out.data;

  // clang-format off

  float filter[9] = { //ここの数字いじると結果が変わる 合計0でHPF 1以上でLPF
     0,  -1,   0, 
    -1,   5,  -1, 
     0,  -1,   0
  };

  // clang-format on

  auto filter3x3 = [](uint8_t *p, const int stride, float *filter,
                      const int N2) {
    int idx_filter = 0;
    float val = 0.0F;
    for (int hy = -N2; hy <= N2; ++hy) {
      for (int hx = -N2; hx <= N2; ++hx) {
        int idx_local = hy * stride + hx;
        val += p[idx_local] * filter[idx_filter];
        idx_filter++;
      }
    }
    return val;
  };

  for (int y = N2; y < H - N2; ++y) {
    for (int x = N2; x < W - N2; ++x) {
      int idx = y * W + x;
      uint8_t *in_local = &in[idx];
      float val = filter3x3(in_local, W, filter, N2);
      // val /= N * N;  // N*Nでフィルターかけ(ぼける)
      if (val < 0.0F) {
        val = 0.0F;
      }
      if (val > 225.0F) {
        val = 255.0F;
      }

      out[idx] = (uint8_t)val;
    }
  }

  //  画像の表示
  cv::imshow("Original", img);
  cv::imshow("Filtered", img_out);

  // キー入力を待つ
  cv::waitKey();
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}

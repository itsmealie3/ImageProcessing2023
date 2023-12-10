#include <cmath>
#include <cstdio>  // 画像を変更したいときはlaunch.jsonからargs[./data/！ここ変えて！.pgm]
#include <cstdlib>
#include <opencv2/opencv.hpp>

int clip(int v);

void myBGR2YCbCr(cv::Mat &in, cv::Mat &out);
void myYCbCr2BGR(cv::Mat &in, cv::Mat &out);
void quantize(cv::Mat &in, float stepsize);

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

  cv::Mat YCbCr = img.clone();
  cv::Mat img2 = img.clone();

  myBGR2YCbCr(img, YCbCr);
  quantize(YCbCr, 32.0);
  myYCbCr2BGR(YCbCr, img2);

  //  画像の表示
  cv::imshow("RGB", img);
  cv::imshow("YCbCr2BGR", img2);

  // キー入力を待つ
  cv::waitKey();
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}

int clip(int v) {
  if (v > 255) {
    v = 255;
  }
  if (v < 0) {
    v = 0;
  }
  return v;
}

void myBGR2YCbCr(cv::Mat &in, cv::Mat &out) {  // 変換
  int W = in.cols;
  int H = in.rows;
  int nc = in.channels();

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < nc * W; x += nc) {
      int idx = y * nc * W + x;

      int B = in.data[idx];
      int G = in.data[idx + 1];
      int R = in.data[idx + 2];

      int Y = 0.299 * R + 0.587 * G + 0.114 * B;
      int Cb = -0.169 * R - 0.331 * G + 0.5 * B;
      int Cr = 0.5 * R - 0.419 * G - 0.081 * B;

      out.data[idx] = clip(Y);
      out.data[idx + 1] = clip(Cb + 128);
      out.data[idx + 2] = clip(Cr + 128);
    }
  }
}

void myYCbCr2BGR(cv::Mat &in, cv::Mat &out) {  // 逆変換
  int W = in.cols;
  int H = in.rows;
  int nc = in.channels();

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < nc * W; x += nc) {
      int idx = y * nc * W + x;

      int Y = in.data[idx];
      int Cb = in.data[idx + 1] - 128;
      int Cr = in.data[idx + 2] - 128;

      int R = Y + 1.402 * Cr;
      int G = Y - 0.344 * Cb - 0.714 * Cr;
      int B = Y + 1.772 * Cb;

      out.data[idx] = clip(B);
      out.data[idx + 1] = clip(G);
      out.data[idx + 2] = clip(R);
    }
  }
}

void quantize(cv::Mat &in, float stepsize) {  // 逆変換
  int W = in.cols;
  int H = in.rows;
  int nc = in.channels();

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < nc * W; x += nc) {
      int idx = y * nc * W + x;

      int Y = in.data[idx];
      int Cb = in.data[idx + 1];
      int Cr = in.data[idx + 2];

      // 量子化と逆量子化
      // Y = floor((floor(Y / stepsize) + 0.5) * stepsize); //輝度は量子化しない
      Cb = floor((floor(Cb / stepsize) + 0.5) * stepsize);
      Cr = floor((floor(Cr / stepsize) + 0.5) * stepsize);

      in.data[idx] = clip(Y);
      in.data[idx + 1] = clip(Cb);
      in.data[idx + 2] = clip(Cr);
    }
  }
}
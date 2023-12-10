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

// ブロック処理
void blkproc(cv::Mat &in, std::function<void(cv::Mat &)> func) {
  for (int y = 0; y < in.rows; y += DCTSIZE) {
    for (int x = 0; x < in.cols; x += DCTSIZE) {
      cv::Mat blk_in, blk_out;
      blk_in = in(cv::Rect(x, y, DCTSIZE, DCTSIZE)).clone();
      blk_out = in(cv::Rect(x, y, DCTSIZE, DCTSIZE));
      func(blk_in);
      blk_in.convertTo(blk_out, blk_out.type());
    }
  }
}

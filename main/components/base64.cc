#include <string>

static constexpr unsigned char kDecodingTable[] = {
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
  64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
  15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
  64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

std::string base64_decode(const std::string &input) {
  size_t in_len = input.size();
  if (in_len % 4 != 0) return "";

  size_t out_len = in_len / 4 * 3;
  if (input[in_len - 1] == '=') out_len--;
  if (input[in_len - 2] == '=') out_len--;

  std::string out(out_len, '\0');
  for (size_t i = 0, j = 0; i < in_len;) {
      uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[(int)input[i++]];
      uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[(int)input[i++]];
      uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[(int)input[i++]];
      uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[(int)input[i++]];

      uint32_t triple = (a << 18) + (b << 12) + (c << 6) + d;

      if (j < out_len) out[j++] = (triple >> 16) & 0xFF;
      if (j < out_len) out[j++] = (triple >> 8) & 0xFF;
      if (j < out_len) out[j++] = triple & 0xFF;
  }
  return out;
}

static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const unsigned char* data, size_t len) {
  std::string encoded;
  encoded.reserve(((len + 2) / 3) * 4);

  for (size_t i = 0; i < len;) {
      uint32_t octet_a = i < len ? data[i++] : 0;
      uint32_t octet_b = i < len ? data[i++] : 0;
      uint32_t octet_c = i < len ? data[i++] : 0;

      uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

      encoded.push_back(encoding_table[(triple >> 18) & 0x3F]);
      encoded.push_back(encoding_table[(triple >> 12) & 0x3F]);
      encoded.push_back(i > len + 1 ? '=' : encoding_table[(triple >> 6) & 0x3F]);
      encoded.push_back(i > len ? '=' : encoding_table[triple & 0x3F]);
  }

  return encoded;
}
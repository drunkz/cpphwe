#ifndef HW_INTERNALS_H
#define HW_INTERNALS_H

#define RECV_BUF_SIZE 8192

namespace hw {

enum InitFlags : unsigned short { kInitDefault = 1 << 0, kInitUV = 1 << 1, kInitDB = 1 << 2, kInitV8 = 1 << 3 };

} // namespace hw

#endif // HW_INTERNALS_H
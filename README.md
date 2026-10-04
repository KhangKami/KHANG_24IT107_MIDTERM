# Bài tập giữa kỳ: Cài đặt lệnh ls

- Sinh viên: Nguyễn Công Tuệ Khang
- Mã sinh viên: 24IT107
- Môn học: Lập trình hệ thống
- Nền tảng: NetBSD 10.1, trình biên dịch gcc 10.5
- GitHub: https://github.com/KhangKami/KHANG_24IT107_MIDTERM

## 1. Giới thiệu

Đây là phiên bản đơn giản của lệnh `ls(1)` trong UNIX, viết bằng ngôn ngữ C
từ đầu, bám theo trang man của NetBSD mà đề bài cung cấp. Chương trình chỉ
hỗ trợ tập con các tùy chọn nằm trong trang man đó.

## 2. Cách biên dịch và chạy

    make            # biên dịch, tạo file thực thi ./myls
    make clean      # xóa các file .o và file thực thi
    ./myls [-AacdFfhiklnqRrSstuw] [file ...]

Ví dụ:

    ./myls                  # liệt kê thư mục hiện tại
    ./myls -la /etc         # dạng chi tiết, có cả file ẩn
    ./myls -lhS src         # kích thước dễ đọc, xếp theo kích thước
    ./myls -R .             # liệt kê đệ quy
    ./myls -d src include   # chỉ in tên thư mục, không đi vào trong

## 3. Các tùy chọn đã cài đặt

| Tùy chọn | Chức năng |
|---|---|
| -A | Liệt kê tất cả trừ `.` và `..` |
| -a | Liệt kê cả các mục bắt đầu bằng dấu chấm |
| -c | Dùng thời gian đổi trạng thái file (thay cho thời gian sửa) khi sắp xếp -t hoặc in -l |
| -u | Dùng thời gian truy cập cuối (thay cho thời gian sửa) khi sắp xếp -t hoặc in -l |
| -d | Thư mục được coi như file thường, không liệt kê nội dung bên trong |
| -F | Thêm ký hiệu sau tên: `/` thư mục, `*` file thực thi, `@` symlink, `%` whiteout, `=` socket, `\|` FIFO |
| -f | Không sắp xếp |
| -h | Kích thước dạng dễ đọc (1.5K, 23M...), dùng với -l và -s |
| -i | In số inode của từng file |
| -k | Số block tính theo kilobyte, dùng với -s |
| -l | Dạng danh sách dài |
| -n | Giống -l nhưng owner và group hiển thị dạng số |
| -q | Ký tự không in được trong tên file hiển thị là `?` (mặc định khi xuất ra terminal) |
| -w | In nguyên ký tự không in được (mặc định khi xuất ra file hoặc pipe) |
| -R | Liệt kê đệ quy các thư mục con |
| -r | Đảo ngược thứ tự sắp xếp |
| -S | Sắp xếp theo kích thước, file lớn nhất trước |
| -s | In số block mà mỗi file chiếm (đơn vị 512 byte hoặc theo biến `BLOCKSIZE`) |
| -t | Sắp xếp theo thời gian, file mới nhất trước |

Với các cặp tùy chọn ghi đè nhau (-l/-n, -c/-u, -q/-w, -h/-k), tùy chọn
**nằm bên phải nhất** sẽ được áp dụng.

## 4. Hành vi chi tiết

- Không có đối số: liệt kê thư mục hiện tại.
- Nhiều đối số: các đối số không phải thư mục được in trước, sau đó đến các
  thư mục. Hai nhóm được sắp xếp riêng theo thứ tự từ điển.
- Khi có nhiều hơn một đối số, mỗi thư mục có dòng tiêu đề `tên:` và các
  khối cách nhau một dòng trống. Với `-R`, các thư mục con cũng có tiêu đề.
- Dạng `-l` gồm: kiểu file và quyền, số link, owner, group, kích thước (byte),
  ngày giờ sửa đổi, tên. Với symlink in thêm `-> đích`. Với file thiết bị,
  cột kích thước hiển thị `major, minor`.
- Dòng `total N` được in trước danh sách của mỗi thư mục khi dùng `-l`
  (và khi dùng `-s` nếu xuất ra terminal). Thư mục rỗng không in dòng này.
- Ngày giờ: file sửa trong vòng sáu tháng hiển thị giờ phút, file cũ hơn hoặc
  ở tương lai hiển thị năm.
- Biến môi trường `BLOCKSIZE` được dùng khi không có -h và -k.
- Lỗi (file không tồn tại, không có quyền đọc...) được in ra stderr, chương
  trình vẫn xử lý tiếp các đối số còn lại. Mã thoát là 0 nếu thành công,
  1 nếu có lỗi.
- Khi `-R`, chương trình không đi theo symlink trỏ tới thư mục, nên không bị
  lặp vô hạn.

## 5. Cấu trúc project

Mã nguồn được chia module theo chức năng:

| File | Vai trò |
|---|---|
| src/main.c | Điểm vào chương trình |
| src/options.c, include/options.h | Phân tích tùy chọn bằng getopt, struct lưu các cờ |
| src/entry.c, include/entry.h | Danh sách các file (dùng lstat), mảng động |
| src/list.c, include/list.h | Đọc nội dung thư mục (opendir/readdir), lọc -a/-A |
| src/sort.c, include/sort.h | Sắp xếp bằng qsort: tên, -t, -S, -r, -f |
| src/format.c, include/format.h | Định dạng chuỗi quyền, kích thước, block, ký hiệu -F |
| src/print.c, include/print.h | In dạng ngắn và dạng dài |
| src/ls.c, include/ls.h | Xử lý đối số, thư mục, đệ quy -R |
| src/utils.c, include/utils.h | malloc/realloc/strdup an toàn |

## 6. Kiểm thử

Kết quả của `myls` được so sánh với lệnh `ls` có sẵn của NetBSD trên một cây
thư mục thử gồm: thư mục rỗng, thư mục lồng nhau, symlink đúng và symlink
hỏng, FIFO, file setuid, thư mục sticky, file ẩn, tên file bắt đầu bằng `-`.

Các trường hợp biên đã kiểm tra: file không tồn tại, tên rỗng, thư mục không
có quyền đọc, nhiều đối số trong đó có đối số lỗi, file thiết bị, và chạy
đệ quy trên `/etc` (có thư mục không đọc được) mà không bị lỗi segmentation
fault.

## 7. Điểm khác biệt đã biết so với ls thật

- Thông báo lỗi bắt đầu bằng `myls:` thay vì `ls:`.
- Symlink trỏ tới thư mục khi được đưa vào làm đối số sẽ không bị đi theo.

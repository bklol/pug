chcp 65001
if not exist build mkdir build
cd build
python ../configure.py --enable-optimize --sdks=cs2 --targets=x86_64 --mms_path=C:\Users\Administrator\Desktop\hl2sdk-root\metamod --hl2sdk-root=C:\Users\Administrator\Desktop\hl2sdk-root --hl2sdk-manifests C:\Users\Administrator\Desktop\hl2sdk-root\hl2sdk-manifests
ambuild
pause()
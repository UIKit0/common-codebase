py2exe��Python����ת����exe��ִ�г����С����

py2exe��һ��python�������ߣ����԰�python�ű�ת����windows�µĿ�ִ�г��򣬲���Ҫ��װpython������С�
py2exe���ڿ�����������ʹ����wxPython, Tkinter, Pmw, PyGTK, pygame, win32com client and server ��ģ��ĳ���

��ϸ���ܿ��Կ����Ĺٷ���վ
http://starship.python.net/crew/theller/py2exe/ ����www.py2exe.org

1. ���ذ�װpy2exe

    py2exeĿǰ�İ汾��0.6.9�������㰲װ��python�İ汾ѡ�����ص��ļ�
    ����������py2exe-0.6.9.win32-py2.6.exe

    ��װ����ļ�Ӧ�������python��װĿ¼�µ�Lib\site-packages\py2exe

2.  ʹ��py2exe
������׼��һ���򵥵�python����hello.py

    # hello.py
    def main():
      print "Hello, World!"

    if __name__ == '__main__':
      main()  

Ȼ��Ϊʹ��py2exeдһ���ű�setup.py

    # setup.py
    from distutils.core import setup
    import py2exe

    setup(console=["hello.py"])

 

����setup.py���ǵ�Ҫ��һ����������

    python setup.py py2exe

Ӧ�ÿ���һЩ�����Ϣ
���ã�running py2exe
creating E:\Projects\WorkSpace\Python\build
creating E:\Projects\WorkSpace\Python\build\bdist.win32
creating E:\Projects\WorkSpace\Python\build\bdist.win32\winexe
creating E:\Projects\WorkSpace\Python\build\bdist.win32\winexe\collect
creating E:\Projects\WorkSpace\Python\build\bdist.win32\winexe\temp
creating E:\Projects\WorkSpace\Python\dist
*** searching for required modules ***
*** parsing results ***
creating python loader for extension '_sre'
*** finding dlls needed ***
*** create binaries ***
*** byte compile python files ***
byte-compiling C:\Python23\lib\copy_reg.py to copy_reg.pyc
byte-compiling C:\Python23\lib\sre_compile.py to sre_compile.pyc
byte-compiling E:\Projects\WorkSpace\Python\build\bdist.win32\winexe\temp\_sre.py to _sre.pyc
byte-compiling C:\Python23\lib\macpath.py to macpath.pyc
byte-compiling C:\Python23\lib\popen2.py to popen2.pyc
byte-compiling C:\Python23\lib\atexit.py to atexit.pyc
byte-compiling C:\Python23\lib\os2emxpath.py to os2emxpath.pyc
byte-compiling C:\Python23\lib\sre_constants.py to sre_constants.pyc
byte-compiling C:\Python23\lib\re.py to re.pyc
byte-compiling C:\Python23\lib\ntpath.py to ntpath.pyc
byte-compiling C:\Python23\lib\stat.py to stat.pyc
byte-compiling C:\Python23\lib\string.py to string.pyc
byte-compiling C:\Python23\lib\warnings.py to warnings.pyc
byte-compiling C:\Python23\lib\UserDict.py to UserDict.pyc
byte-compiling C:\Python23\lib\repr.py to repr.pyc
byte-compiling C:\Python23\lib\copy.py to copy.pyc
byte-compiling C:\Python23\lib\types.py to types.pyc
byte-compiling C:\Python23\lib\posixpath.py to posixpath.pyc
byte-compiling C:\Python23\lib\sre.py to sre.pyc
byte-compiling C:\Python23\lib\linecache.py to linecache.pyc
byte-compiling C:\Python23\lib\sre_parse.py to sre_parse.pyc
byte-compiling C:\Python23\lib\os.py to os.pyc
*** copy extensions ***
copying C:\Python23\DLLs\_sre.pyd ->; E:\Projects\WorkSpace\Python\dist
*** copy dlls ***
py2exe���ڵ�ǰĿ¼����������Ŀ¼ build��dist
build����һЩpy2exe����ʱ�������м��ļ���dist�������յĿ�ִ���ļ�

    library.zip
     w9xpopen.exe
     python23.dll
     hello.exe

���ڿ�������hello.exe��

    E:\Projects\WorkSpace\Python\dist>hello
    Hello, World!

�����ǵ����Ҫ��������Ļ�����ʱ��library.zip�� w9xpopen.exe��python23.dll�⼸���ļ��Ǳ���Ҫ��hello.exe��һ��ġ�

���ˣ�����ȵ�����´�������һ��wxPython������

��󣬴����������

    python setup.py py2exe --help

����py2exe������Щ����
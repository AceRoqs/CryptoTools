_Configurations_ is a shared set of Visual Studio property sheets for common
build settings and simple integration of shared libraries.  In the future,
other types of configurations (i.e. ClangFormat) will be placed here.

* _Artifacts_ - Contains variables that can be used for output artifacts.
* _CRTWarnings.Disable_ - Adds preprocessor macros that disable warnings for
usage of unsafe CRT library calls.
* _DirectX9_ - Adds path information to access the DirectX9 SDK.
* _ImageProcessing/PortableRuntime/WindowsCommon_ - Adds include path information
and .lib files for building against these shared libraries.
* _Project.Default_ - Contains shared build parameters.
* _Project2.Default_ - Contains shared build parameters that depend on user macros.

Toby Jones \([www.turbohex.com](http://www.turbohex.com), [ace.roqs.net](http://ace.roqs.net)\)

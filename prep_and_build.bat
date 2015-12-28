call env.bat
call C:\Program Files/ (x86)\Microsoft/ Visual/ Studio 12.0\Common7\Tools\vsvars32.bat
rd /s /q build_win_vc2013
call prep_vc2013.bat
msbuild build_win_vc2013\projects\WebChimera\WebChimera.sln /property:Configuration=MinSizeRel  /maxcpucount:8

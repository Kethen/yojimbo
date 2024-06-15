set -xe


rm -rf dist
mkdir dist

for arch in x86_64 i686
do
	CPPC=${arch}-w64-mingw32-g++
	mkdir dist/${arch}

	$CPPC -g -shared -o dist/${arch}/sodium.dll -fPIC sodium/*.c -lntdll -Wl,-Bstatic -static-libgcc -static-libstdc++
	$CPPC -g -shared -o dist/${arch}/netcode.dll -Isodium netcode/*.c -lntdll -Ldist/${arch} -lsodium -lws2_32 -Wl,-Bstatic -static-libgcc -static-libstdc++
	$CPPC -g -shared -o dist/${arch}/reliable.dll reliable/*.c -lntdll -Wl,-Bstatic -static-libgcc -static-libstdc++
	$CPPC -g -shared -o dist/${arch}/tlsf.dll tlsf/*.c -lntdll -Wl,-Bstatic -static-libgcc -static-libstdc++
	$CPPC -g -shared -o dist/${arch}/yojimbo.dll -Iinclude -Iserialize -Inetcode -Ireliable -Isodium -I./ source/*.cpp -DYOJIMBO_DEBUG=1 -Ldist/${arch} -lws2_32 -lntdll -lnetcode -lreliable -ltlsf -lsodium -Wl,-Bstatic -static-libgcc -static-libstdc++

done

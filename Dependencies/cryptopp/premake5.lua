--https://git.doszgep.hu/crys/mtasa-blue/blob/bfbab7e733405c0a600d4f86c26f5c169f4b9a0d/vendor/cryptopp/premake5.lua
--https://stackoverflow.com/questions/24174063/correct-way-of-linking-crypto-library-with-my-application
project "cryptopp"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		"CRYPTOPP_DISABLE_ASM", -- disable assembly files x64dll.asm, x64masm.asm and rdrand.as
		--"CRYPTOPP_ENABLE_NAMESPACE_WEAK", -- enable only when using MD5 to disable warning 'You may be using a weak algorithm that has been retained for backwards compatibility...'
		--"CRYPTOPP_IMPORTS", -- for static build
		--"CRYPTOPP_DLL",  -- for dynamic build
	}

	-- Enable SSE2 vector processing
	--vectorextensions "SSE2"

	
	files
	{
	
		--https://stackoverflow.com/questions/54388944/linker-error-while-building-crypto-with-qt-program
		-- Source
		"cryptlib.cpp", "cpu.cpp", "integer.cpp", 
		--"way.cpp",
		"adler32.cpp", "algebra.cpp", "algparam.cpp", 
		"arc4.cpp", "aria.cpp", "aria_simd.cpp", "ariatab.cpp", "asn.cpp", "authenc.cpp", "base32.cpp", 
		"base64.cpp", "basecode.cpp", "bfinit.cpp", "blake2.cpp", "blake2b_simd.cpp", "blake2s_simd.cpp", 
		"blowfish.cpp", "blumshub.cpp", "camellia.cpp", "cast.cpp", "casts.cpp", "cbcmac.cpp", "ccm.cpp",
		"chacha.cpp", "chacha_avx.cpp", "chacha_simd.cpp", "cham.cpp", "cham_simd.cpp", "channels.cpp", "cmac.cpp", 
		"crc.cpp", "crc_simd.cpp", "darn.cpp", "default.cpp", "des.cpp", "dessp.cpp", "dh.cpp", "dh2.cpp", "dll.cpp",
		"donna_32.cpp", "donna_64.cpp", "donna_sse.cpp", "dsa.cpp", "eax.cpp", "ec2n.cpp", "eccrypto.cpp", 
		"ecp.cpp", 
		--"elgamal.cpp",
		"emsa2.cpp", "eprecomp.cpp", "esign.cpp", "files.cpp", "filters.cpp",
		"fips140.cpp", "fipstest.cpp", "gcm.cpp", "gcm_simd.cpp", "gf256.cpp", "gf2_32.cpp", "gf2n.cpp",
		"gf2n_simd.cpp", "gfpcrypt.cpp", "gost.cpp", "gzip.cpp", "hc128.cpp", "hc256.cpp", "hex.cpp", "hight.cpp", 
		"hmac.cpp", "hrtimer.cpp", "ida.cpp", "idea.cpp", "iterhash.cpp", "kalyna.cpp", "kalynatab.cpp", "keccak.cpp",
		--"keccakc.cpp",
		"lea.cpp", "lea_simd.cpp", 
		--"lsh.cpp",
		"luc.cpp", "mars.cpp", "marss.cpp", "md2.cpp", 
		"md4.cpp", "md5.cpp", "misc.cpp", "modes.cpp", "mqueue.cpp", "mqv.cpp", "nbtheory.cpp", "neon_simd.cpp", 
		"oaep.cpp",
		--"ospstore.cpp",
		"osrng.cpp", "padlkrng.cpp", "panama.cpp", "pkcspad.cpp", "poly1305.cpp", 
		"polynomi.cpp", "ppc_power7.cpp", "ppc_power8.cpp", "ppc_power9.cpp", "ppc_simd.cpp", "pssr.cpp",
		"pubkey.cpp", "queue.cpp", "rabbit.cpp", "rabin.cpp", "randpool.cpp", "rc2.cpp", "rc5.cpp", "rc6.cpp",
		"rdrand.cpp", "rdtables.cpp", "rijndael.cpp", "rijndael_simd.cpp", "ripemd.cpp", "rng.cpp", "rsa.cpp", 
		"rw.cpp", "safer.cpp", "salsa.cpp", "scrypt.cpp", "seal.cpp", "seed.cpp", "serpent.cpp", "sha.cpp", "sha3.cpp",
		"sha_simd.cpp", "shacal2.cpp", "shacal2_simd.cpp", "shark.cpp", "sharkbox.cpp", "simeck.cpp",
		"simeck_simd.cpp", "simon.cpp", "simon128_simd.cpp", "simon64_simd.cpp", "skipjack.cpp", 
		"sm3.cpp", "sm4.cpp", "sm4_simd.cpp", "sosemanuk.cpp", "speck.cpp", "speck128_simd.cpp",
		"speck64_simd.cpp", "square.cpp", "squaretb.cpp", "sse_simd.cpp", "strciphr.cpp", "tea.cpp", "tftables.cpp",
		"threefish.cpp", "tiger.cpp", "tigertab.cpp", "ttmac.cpp", "tweetnacl.cpp", "twofish.cpp", "vmac.cpp",
		"wake.cpp", "whrlpool.cpp", "xed25519.cpp", "xtr.cpp", "xtrcrypt.cpp", "zdeflate.cpp", "zinflate.cpp", "zlib.cpp",

		-- Header
		"3way.h", "adler32.h", "adv_simd.h", "aes.h", "aes_armv4.h", "algebra.h", "algparam.h", "androidpay.h",
		"arc4.h", "argnames.h", "aria.h", "arm_simd.h", "asn.h", "authenc.h", "base32.h", "base64.h", "basecode.h",
		 "blake2.h", "blowfish.h", "blumshub.h", "camellia.h", "cast.h", "cbcmac.h", "ccm.h", "chacha.h", "cham.h",
		 "channels.h", "cmac.h", "config.h", "cpu.h", "crc.h", "cryptlib.h", "darn.h", "default.h", "des.h", "dh.h", "dh2.h",
		 "dll.h", "dmac.h", "donna.h", "donna_32.h", "donna_64.h", "donna_sse.h", "drbg.h", "dsa.h", "eax.h", 
		 "ec2n.h", "eccrypto.h", "ecp.h", "ecpoint.h",
		 --"elgamal.h",
		 "emsa2.h", "eprecomp.h", "equihash.h", "esign.h",
		 "fhmqv.h", "files.h", "filters.h", "fips140.h", "fltrimpl.h", "gcm.h", "gf256.h", "gf2_32.h", "gf2n.h", 
		 "gfpcrypt.h", "gost.h", "gzip.h", "hashfwd.h", "hc128.h", "hc256.h", "hex.h", "hight.h", "hkdf.h", "hmac.h", 
		 "hmqv.h", "hrtimer.h", "ida.h", "idea.h", "integer.h", "iterhash.h", "kalyna.h", "keccak.h",
		 --"keccakc.h",
		 "lea.h", 
		 --"lsh.h",
		 "lubyrack.h", "luc.h", "mars.h", "md2.h", "md4.h", "md5.h", "mdc.h", "mersenne.h", "misc.h",
		 "modarith.h", "modes.h", "modexppc.h", "mqueue.h", "mqv.h", "naclite.h", "nbtheory.h", "nr.h", "oaep.h", "oids.h",
		 --"ospstore.h",
		 "osrng.h", "ossig.h", "padlkrng.h", "panama.h", "pch.h", "pem.h", "pkcspad.h", "poly1305.h",
		 "polynomi.h", "ppc_simd.h", "pssr.h", "pubkey.h", "pwdbased.h", "queue.h", "rabbit.h", "rabin.h", 
		 "randpool.h", "rc2.h", "rc5.h", "rc6.h", "rdrand.h", "rijndael.h", "ripemd.h", "rng.h", "rsa.h", "rw.h", "safer.h", 
		 "salsa.h", "scrypt.h", "seal.h", "secblock.h", "seckey.h", "seed.h", "serpent.h", "serpentp.h", "sha.h", "sha3.h",
		 "shacal2.h", "shark.h", "simeck.h", "simon.h", "simple.h", "siphash.h", "skipjack.h", "sm3.h", "sm4.h", 
		 "smartptr.h", "sosemanuk.h", "speck.h", "square.h", "stdcpp.h", "strciphr.h", "tea.h", "threefish.h",
		 "tiger.h", "trap.h", "trunhash.h", "ttmac.h", "tweetnacl.h", "twofish.h", "vmac.h", "wake.h", "whrlpool.h", 
		 "words.h", "xed25519.h", "xtr.h", "xtrcrypt.h", "zdeflate.h", "zinflate.h", "zlib.h",

		-- Test sources 
		"adhoccpp", "testcpp", "bench1cpp", "bench2cpp", "bench3cpp", "datatestcpp", "dlltestcpp",
		"fipsalgtcpp", "validat0cpp", "validat1cpp", "validat2cpp", "validat3cpp", "validat4cpp",
		"validat5cpp", "validat6cpp", "validat7cpp", "validat8cpp", "validat9cpp", "validat10cpp",
		"regtest1cpp", "regtest2cpp", "regtest3cpp", "regtest4cpp",

		 -- Test headers 
		"bench.h", "factory.h", "validate.h"
	}

	filter "platforms:x64"
		files 
		{
			-- Enable if CRYPTOPP_DISABLE_ASM not defined above
			--"x64dll.asm",
			--"x64masm.asm"
		}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_WINSOCK_DEPRECATED_NO_WARNINGS",
		}
		links 
	 	{
	      --[["advapi32", "kernel32", "user32", "gdi32", "comdlg32",
	      "shell32", "windowscodecs", "comctl32", "msimg32",
	      "winspool", "wininet", "urlmon", "gdiplus", "ole32",
	      "oleAut32", "shlwapi", "version", "crypt32"
	      --]]
	    }
	filter "system:linux"
		defines
		{
		}
		links 
	 	{
	    }
	filter "system:macosx"
		defines
		{
		}
		links 
	 	{
	    }


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full"

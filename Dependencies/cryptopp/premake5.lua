--https://git.doszgep.hu/crys/mtasa-blue/blob/bfbab7e733405c0a600d4f86c26f5c169f4b9a0d/vendor/cryptopp/premake5.lua
--https://stackoverflow.com/questions/24174063/correct-way-of-linking-crypto-library-with-my-application
project "cryptopp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11" -- requires c++11 to build correctly
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		--[[
		Define this to disable ASM, intrinsics and built-ins. The code will be 
		compiled using C++ only. The library code will not include SSE2 (and above),
		NEON, Aarch32, Aarch64, Power4, Power7 or Power8.
		#define CRYPTOPP_DISABLE_ASM 1
		]] 
		"CRYPTOPP_DISABLE_ASM=1",

		--"CRYPTOPP_ENABLE_NAMESPACE_WEAK", -- enable only when using MD5 to disable warning 'You may be using a weak algorithm that has been retained for backwards compatibility...'
		--"CRYPTOPP_IMPORTS", -- for static build
		--"CRYPTOPP_EXPORTS",  -- for dynamic build
	}

	-- Enable SSE2 vector processing https://github.com/premake/premake-core/wiki/vectorextensions
	--vectorextensions "SSE2"


	files
	{
		"*.h",
		"*.cpp",
	}
--[[
	files
	{

		-- Headers
		"3way.h", 		"adler32.h", 		"adv_simd.h", 		"aes.h", 		"aes_armv4.h", 		"algebra.h", 		"algparam.h", 		"allocate.h", 
		"arc4.h", 		"argnames.h", 		"aria.h", 		"arm_simd.h", 		"asn.h", 		"authenc.h", 		"base32.h", 		"base64.h", 
		"basecode.h", 		"bench.h", 		"blake2.h", 		"blowfish.h", 		"blumshub.h", 		"camellia.h", 		"cast.h", 		"cbcmac.h", 
		"ccm.h", 		"chacha.h", 		"chachapoly.h", 		"cham.h", 		"channels.h", 		"cmac.h", 		"config.h", 		"config_align.h", 
		"config_asm.h", 		"config_cpu.h", 		"config_cxx.h", 		"config_dll.h", 		"config_int.h", 		"config_misc.h", 		"config_ns.h", 		"config_os.h", 
		"config_ver.h", 		"cpu.h", 		"crc.h", 		"cryptlib.h", 		"darn.h", 		"default.h", 		"des.h", 		"dh.h", 
		"dh2.h", 		"dll.h", 		"dmac.h", 		"donna.h", 		"donna_32.h", 		"donna_64.h", 		"donna_sse.h", 		"drbg.h", 
		"dsa.h", 		"eax.h", 		"ec2n.h", 		"eccrypto.h", 		"ecp.h", 		"ecpoint.h", 		"elgamal.h", 		"emsa2.h", 
		"eprecomp.h", 		"esign.h", 		"factory.h", 		"fhmqv.h", 		"files.h", 		"filters.h", 		"fips140.h", 		"fltrimpl.h", 
		"gcm.h", 		"gf256.h", 		"gf2n.h", 		"gf2_32.h", 		"gfpcrypt.h", 		"gost.h", 		"gzip.h", 		"hashfwd.h", 
		"hc128.h", 		"hc256.h", 		"hex.h", 		"hight.h", 		"hkdf.h", 		"hmac.h", 		"hmqv.h", 		"hrtimer.h", 
		"ida.h", 		"idea.h", 		"integer.h", 		"iterhash.h", 		"kalyna.h", 		"keccak.h", 		"lea.h", 		"lubyrack.h", 
		"luc.h", 		"mars.h", 		"md2.h", 		"md4.h", 		"md5.h", 		"mdc.h", 		"mersenne.h", 		"misc.h", 
		"modarith.h", 		"modes.h", 		"modexppc.h", 		"mqueue.h", 		"mqv.h", 		"naclite.h", 		"nbtheory.h", 		"nr.h", 
		"oaep.h", 		"oids.h", 		"osrng.h", 		"ossig.h", 		"padlkrng.h", 		"panama.h", 		"pch.h", 		"pkcspad.h", 
		"poly1305.h", 		"polynomi.h", 		"ppc_simd.h", 		"pssr.h", 		"pubkey.h", 		"pwdbased.h", 		"queue.h", 		"rabbit.h", 
		"rabin.h", 		"randpool.h", 		"rc2.h", 		"rc5.h", 		"rc6.h", 		"rdrand.h", 		"resource.h", 		"rijndael.h", 
		"ripemd.h", 		"rng.h", 		"rsa.h", 		"rw.h", 		"safer.h", 		"salsa.h", 		"scrypt.h", 		"seal.h", 
		"secblock.h", 		"secblockfwd.h", 		"seckey.h", 		"seed.h", 		"serpent.h", 		"serpentp.h", 		"sha.h", 		"sha1_armv4.h", 
		"sha256_armv4.h", 		"sha3.h", 		"sha512_armv4.h", 		"shacal2.h", 		"shake.h", 		"shark.h", 		"simeck.h", 		"simon.h", 
		"simple.h", 		"siphash.h", 		"skipjack.h", 		"sm3.h", 		"sm4.h", 		"smartptr.h", 		"sosemanuk.h", 		"speck.h", 
		"square.h", 		"stdcpp.h", 		"strciphr.h", 		"tea.h", 		"threefish.h", 		"tiger.h", 		"trap.h", 		"trunhash.h", 
		"ttmac.h", 		"tweetnacl.h", 		"twofish.h", 		"validate.h", 		"vmac.h", 		"wake.h", 		"whrlpool.h", 		"words.h", 
		"xed25519.h", 		"xtr.h", 		"xtrcrypt.h", 		"xts.h", 		"zdeflate.h", 		"zinflate.h", 		"zlib.h", 

		-- Sources
		"3way.cpp", 		"adler32.cpp", 		"algebra.cpp", 		"algparam.cpp", 		"allocate.cpp", 		"arc4.cpp", 		"aria.cpp", 		"ariatab.cpp", 
		"aria_simd.cpp", 		"asn.cpp", 		"authenc.cpp", 		"base32.cpp", 		"base64.cpp", 		"basecode.cpp", 		"bench1.cpp", 		"bench2.cpp", 
		"bench3.cpp", 		"bfinit.cpp", 		"blake2.cpp", 		"blake2b_simd.cpp", 		"blake2s_simd.cpp", 		"blowfish.cpp", 		"blumshub.cpp", 		"camellia.cpp", 
		"cast.cpp", 		"casts.cpp", 		"cbcmac.cpp", 		"ccm.cpp", 		"chacha.cpp", 		"chachapoly.cpp", 		"chacha_avx.cpp", 		"chacha_simd.cpp", 
		"cham.cpp", 		"cham_simd.cpp", 		"channels.cpp", 		"cmac.cpp", 		"cpu.cpp", 		"crc.cpp", 		"crc_simd.cpp", 		"cryptlib.cpp", 
		"darn.cpp", 		"datatest.cpp", 		"default.cpp", 		"des.cpp", 		"dessp.cpp", 		"dh.cpp", 		"dh2.cpp", 		"dll.cpp", 
		"dlltest.cpp", 		"donna_32.cpp", 		"donna_64.cpp", 		"donna_sse.cpp", 		"dsa.cpp", 		"eax.cpp", 		"ec2n.cpp", 		"eccrypto.cpp", 
		"ecp.cpp", 		"elgamal.cpp", 		"emsa2.cpp", 		"eprecomp.cpp", 		"esign.cpp", 		"files.cpp", 		"filters.cpp", 		"fips140.cpp", 
		"fipsalgt.cpp", 		"fipstest.cpp", 		"gcm.cpp", 		"gcm_simd.cpp", 		"gf256.cpp", 		"gf2n.cpp", 		"gf2n_simd.cpp", 		"gf2_32.cpp", 
		"gfpcrypt.cpp", 		"gost.cpp", 		"gzip.cpp", 		"hc128.cpp", 		"hc256.cpp", 		"hex.cpp", 		"hight.cpp", 		"hmac.cpp", 
		"hrtimer.cpp", 		"ida.cpp", 		"idea.cpp", 		"integer.cpp", 		"iterhash.cpp", 		"kalyna.cpp", 		"kalynatab.cpp", 		"keccak.cpp", 
		"keccak_core.cpp", 		"keccak_simd.cpp", 		"lea.cpp", 		"lea_simd.cpp", 		"luc.cpp", 		"mars.cpp", 		"marss.cpp", 		"md2.cpp", 
		"md4.cpp", 		"md5.cpp", 		"misc.cpp", 		"modes.cpp", 		"mqueue.cpp", 		"mqv.cpp", 		"nbtheory.cpp", 		"neon_simd.cpp", 
		"oaep.cpp", 		"osrng.cpp", 		"padlkrng.cpp", 		"panama.cpp", 		"pch.cpp", 		"pkcspad.cpp", 		"poly1305.cpp", 		"polynomi.cpp", 
		"ppc_power7.cpp", 		"ppc_power8.cpp", 		"ppc_power9.cpp", 		"ppc_simd.cpp", 		"pssr.cpp", 		"pubkey.cpp", 		"queue.cpp", 		"rabbit.cpp", 
		"rabin.cpp", 		"randpool.cpp", 		"rc2.cpp", 		"rc5.cpp", 		"rc6.cpp", 		"rdrand.cpp", 		"rdtables.cpp", 		"regtest1.cpp", 
		"regtest2.cpp", 		"regtest3.cpp", 		"regtest4.cpp", 		"rijndael.cpp", 		"rijndael_simd.cpp", 		"ripemd.cpp", 		"rng.cpp", 		"rsa.cpp", 
		"rw.cpp", 		"safer.cpp", 		"salsa.cpp", 		"scrypt.cpp", 		"seal.cpp", 		"seed.cpp", 		"serpent.cpp", 		"sha.cpp", 
		"sha3.cpp", 		"shacal2.cpp", 		"shacal2_simd.cpp", 		"shake.cpp", 		"shark.cpp", 		"sharkbox.cpp", 		"sha_simd.cpp", 		"simeck.cpp", 
		"simon.cpp", 		"simon128_simd.cpp", 		"simple.cpp", 		"skipjack.cpp", 		"sm3.cpp", 		"sm4.cpp", 		"sm4_simd.cpp", 		"sosemanuk.cpp", 
		"speck.cpp", 		"speck128_simd.cpp", 		"square.cpp", 		"squaretb.cpp", 		"sse_simd.cpp", 		"strciphr.cpp", 		"tea.cpp", 		"test.cpp", 
		"tftables.cpp", 		"threefish.cpp", 		"tiger.cpp", 		"tigertab.cpp", 		"ttmac.cpp", 		"tweetnacl.cpp", 		"twofish.cpp", 		"validat0.cpp", 
		"validat1.cpp", 		"validat10.cpp", 		"validat2.cpp", 		"validat3.cpp", 		"validat4.cpp", 		"validat5.cpp", 		"validat6.cpp", 		"validat7.cpp", 
		"validat8.cpp", 		"validat9.cpp", 		"vmac.cpp", 		"wake.cpp", 		"whrlpool.cpp", 		"xed25519.cpp", 		"xtr.cpp", 		"xtrcrypt.cpp", 
		"xts.cpp", 		"zdeflate.cpp", 		"zinflate.cpp", 		"zlib.cpp", 


		-- Assembly files
		--"rdrand.asm","rdseed.asm",

		 -- Enable if CRYPTOPP_DISABLE_ASM not defined above
		"x64masm.asm", "x64dll.asm"
	}
--]]

	filter "system:windows"
		systemversion "latest"
		files
		{
		}
		defines
		{
			"_WINSOCK_DEPRECATED_NO_WARNINGS",
		}
		links 
	 	{
	 		"ws2_32",
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


	--- Configurations ---
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off" -- No optimization will be performed.

	filter "configurations:Release"
		runtime "Release"
		optimize "On" -- Perform a balanced set of optimizations.
		inlining "Explicit" -- Only inline functions explicitly marked with the inline keyword.

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full" -- Full optimization.
		inlining "Auto" -- Inline any suitable function for full performance

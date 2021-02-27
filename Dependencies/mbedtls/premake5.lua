project "mbedtls"
	kind "StaticLib"
    language "C"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	includedirs 
	{ 
		"include/",
		"library/",
		"3rdparty/everest/include",
		"3rdparty/everest/include/everest",
		"3rdparty/everest/include/everest/kremlib",
	}
	
	defines 
	{
		 --"MBEDTLS_ZLIB_SUPPORT",
		 "KRML_VERIFIED_UINT128"

	}
	
	
	files
	{
		"library/aes.c",
		"library/aesni.c",
		"library/arc4.c",
		"library/aria.c",
		"library/asn1parse.c",
		"library/asn1write.c",
		"library/base64.c",
		"library/bignum.c",
		"library/blowfish.c",
		"library/camellia.c",
		"library/ccm.c",
		"library/certs.c",
		"library/chacha20.c",
		"library/chachapoly.c",
		"library/cipher.c",
		"library/cipher_wrap.c",
		"library/cmac.c",
		"library/ctr_drbg.c",
		"library/debug.c",
		"library/des.c",
		"library/dhm.c",
		"library/ecdh.c",
		"library/ecdsa.c",
		"library/ecjpake.c",
		"library/ecp.c",
		"library/ecp_curves.c",
		"library/entropy.c",
		"library/entropy_poll.c",
		"library/error.c",
		"library/gcm.c",
		"library/havege.c",
		"library/hkdf.c",
		"library/hmac_drbg.c",
		"library/md.c",
		"library/md2.c",
		"library/md4.c",
		"library/md5.c",
		"library/memory_buffer_alloc.c",
		"library/net_sockets.c",
		"library/nist_kw.c",
		"library/oid.c",
		"library/padlock.c",
		"library/pem.c",
		"library/pk.c",
		"library/pk_wrap.c",
		"library/pkcs11.c",
		"library/pkcs12.c",
		"library/pkcs5.c",
		"library/pkparse.c",
		"library/pkwrite.c",
		"library/platform.c",
		"library/platform_util.c",
		"library/poly1305.c",
		"library/psa_crypto.c",
		"library/psa_crypto_driver_wrappers.c",
		"library/psa_crypto_se.c",
		"library/psa_crypto_slot_management.c",
		"library/psa_crypto_storage.c",
		"library/psa_its_file.c",
		"library/ripemd160.c",
		"library/rsa.c",
		"library/rsa_internal.c",
		"library/sha1.c",
		"library/sha256.c",
		"library/sha512.c",
		"library/ssl_cache.c",
		"library/ssl_ciphersuites.c",
		"library/ssl_cli.c",
		"library/ssl_cookie.c",
		"library/ssl_msg.c",
		"library/ssl_srv.c",
		"library/ssl_ticket.c",
		"library/ssl_tls.c",
		"library/ssl_tls13_keys.c",
		"library/threading.c",
		"library/timing.c",
		"library/version.c",
		"library/version_features.c",
		"library/x509.c",
		"library/x509_create.c",
		"library/x509_crl.c",
		"library/x509_crt.c",
		"library/x509_csr.c",
		"library/x509write_crt.c",
		"library/x509write_csr.c",
		"library/xtea.c",
		"3rdparty/everest/library/everest.c",
		"3rdparty/everest/library/Hacl_Curve25519_joined.c",
		"3rdparty/everest/library/x25519.c",
		"3rdparty/everest/library/kremlib/FStar_UInt128_extracted.c",
		"3rdparty/everest/library/kremlib/FStar_UInt64_FStar_UInt32_FStar_UInt16_FStar_UInt8.c",
		"3rdparty/everest/library/legacy/Hacl_Curve25519.c",
		"include/mbedtls/aes.h",
		"include/mbedtls/aesni.h",
		"include/mbedtls/arc4.h",
		"include/mbedtls/aria.h",
		"include/mbedtls/asn1.h",
		"include/mbedtls/asn1write.h",
		"include/mbedtls/base64.h",
		"include/mbedtls/bignum.h",
		"include/mbedtls/blowfish.h",
		"include/mbedtls/bn_mul.h",
		"include/mbedtls/camellia.h",
		"include/mbedtls/ccm.h",
		"include/mbedtls/certs.h",
		"include/mbedtls/chacha20.h",
		"include/mbedtls/chachapoly.h",
		"include/mbedtls/check_config.h",
		"include/mbedtls/cipher.h",
		"include/mbedtls/cipher_internal.h",
		"include/mbedtls/cmac.h",
		"include/mbedtls/compat-1.3.h",
		"include/mbedtls/config.h",
		"include/mbedtls/config_psa.h",
		"include/mbedtls/ctr_drbg.h",
		"include/mbedtls/debug.h",
		"include/mbedtls/des.h",
		"include/mbedtls/dhm.h",
		"include/mbedtls/ecdh.h",
		"include/mbedtls/ecdsa.h",
		"include/mbedtls/ecjpake.h",
		"include/mbedtls/ecp.h",
		"include/mbedtls/ecp_internal.h",
		"include/mbedtls/entropy.h",
		"include/mbedtls/entropy_poll.h",
		"include/mbedtls/error.h",
		"include/mbedtls/gcm.h",
		"include/mbedtls/havege.h",
		"include/mbedtls/hkdf.h",
		"include/mbedtls/hmac_drbg.h",
		"include/mbedtls/md.h",
		"include/mbedtls/md2.h",
		"include/mbedtls/md4.h",
		"include/mbedtls/md5.h",
		"include/mbedtls/md_internal.h",
		"include/mbedtls/memory_buffer_alloc.h",
		"include/mbedtls/net.h",
		"include/mbedtls/net_sockets.h",
		"include/mbedtls/nist_kw.h",
		"include/mbedtls/oid.h",
		"include/mbedtls/padlock.h",
		"include/mbedtls/pem.h",
		"include/mbedtls/pk.h",
		"include/mbedtls/pk_internal.h",
		"include/mbedtls/pkcs11.h",
		"include/mbedtls/pkcs12.h",
		"include/mbedtls/pkcs5.h",
		"include/mbedtls/platform.h",
		"include/mbedtls/platform_time.h",
		"include/mbedtls/platform_util.h",
		"include/mbedtls/poly1305.h",
		"include/mbedtls/psa_util.h",
		"include/mbedtls/ripemd160.h",
		"include/mbedtls/rsa.h",
		"include/mbedtls/rsa_internal.h",
		"include/mbedtls/sha1.h",
		"include/mbedtls/sha256.h",
		"include/mbedtls/sha512.h",
		"include/mbedtls/ssl.h",
		"include/mbedtls/ssl_cache.h",
		"include/mbedtls/ssl_ciphersuites.h",
		"include/mbedtls/ssl_cookie.h",
		"include/mbedtls/ssl_internal.h",
		"include/mbedtls/ssl_ticket.h",
		"include/mbedtls/threading.h",
		"include/mbedtls/timing.h",
		"include/mbedtls/version.h",
		"include/mbedtls/x509.h",
		"include/mbedtls/x509_crl.h",
		"include/mbedtls/x509_crt.h",
		"include/mbedtls/x509_csr.h",
		"include/mbedtls/xtea.h",
		"include/psa/crypto.h",
		"include/psa/crypto_accel_driver.h",
		"include/psa/crypto_compat.h",
		"include/psa/crypto_config.h",
		"include/psa/crypto_driver_common.h",
		"include/psa/crypto_entropy_driver.h",
		"include/psa/crypto_extra.h",
		"include/psa/crypto_platform.h",
		"include/psa/crypto_se_driver.h",
		"include/psa/crypto_sizes.h",
		"include/psa/crypto_struct.h",
		"include/psa/crypto_types.h",
		"include/psa/crypto_values.h",
		"library/check_crypto_config.h",
		"library/common.h",
		"library/psa_crypto_core.h",
		"library/psa_crypto_driver_wrappers.h",
		"library/psa_crypto_invasive.h",
		"library/psa_crypto_its.h",
		"library/psa_crypto_se.h",
		"library/psa_crypto_service_integration.h",
		"library/psa_crypto_slot_management.h",
		"library/psa_crypto_storage.h",
		"library/ssl_invasive.h",
		"library/ssl_tls13_keys.h",
		"3rdparty/everest/include/everest/everest.h",
		"3rdparty/everest/include/everest/Hacl_Curve25519.h",
		"3rdparty/everest/include/everest/kremlib.h",
		"3rdparty/everest/include/everest/x25519.h",
	}

    links
    {
    }

	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
		}
		links
		{
			"ws2_32"
		}


	filter "system:linux"
	    pic "On" -- required fo clang-10
		links
		{
		}
		defines
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

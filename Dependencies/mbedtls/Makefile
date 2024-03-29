# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
DEFINES += -DKRML_VERIFIED_UINT128
INCLUDES += -Iinclude -Ilibrary -I3rdparty/everest/include -I3rdparty/everest/include/everest -I3rdparty/everest/include/everest/kremlib
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS +=
LDDEPS +=
LINKCMD = $(AR) -rcs "$@" $(OBJECTS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug)
TARGETDIR = Bin/Debug-linux-x86_64/mbedtls
TARGET = $(TARGETDIR)/libmbedtls.a
OBJDIR = Bin-Intermediate/Debug-linux-x86_64/mbedtls
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -fPIC -g -w
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O0 -fPIC -g -w
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),release)
TARGETDIR = Bin/Release-linux-x86_64/mbedtls
TARGET = $(TARGETDIR)/libmbedtls.a
OBJDIR = Bin-Intermediate/Release-linux-x86_64/mbedtls
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -fPIC -w
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2 -fPIC -w
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),dist)
TARGETDIR = Bin/Dist-linux-x86_64/mbedtls
TARGET = $(TARGETDIR)/libmbedtls.a
OBJDIR = Bin-Intermediate/Dist-linux-x86_64/mbedtls
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O3 -fPIC -w
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O3 -fPIC -w
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/FStar_UInt128_extracted.o
GENERATED += $(OBJDIR)/FStar_UInt64_FStar_UInt32_FStar_UInt16_FStar_UInt8.o
GENERATED += $(OBJDIR)/Hacl_Curve25519.o
GENERATED += $(OBJDIR)/Hacl_Curve25519_joined.o
GENERATED += $(OBJDIR)/aes.o
GENERATED += $(OBJDIR)/aesni.o
GENERATED += $(OBJDIR)/arc4.o
GENERATED += $(OBJDIR)/aria.o
GENERATED += $(OBJDIR)/asn1parse.o
GENERATED += $(OBJDIR)/asn1write.o
GENERATED += $(OBJDIR)/base64.o
GENERATED += $(OBJDIR)/bignum.o
GENERATED += $(OBJDIR)/blowfish.o
GENERATED += $(OBJDIR)/camellia.o
GENERATED += $(OBJDIR)/ccm.o
GENERATED += $(OBJDIR)/certs.o
GENERATED += $(OBJDIR)/chacha20.o
GENERATED += $(OBJDIR)/chachapoly.o
GENERATED += $(OBJDIR)/cipher.o
GENERATED += $(OBJDIR)/cipher_wrap.o
GENERATED += $(OBJDIR)/cmac.o
GENERATED += $(OBJDIR)/ctr_drbg.o
GENERATED += $(OBJDIR)/debug.o
GENERATED += $(OBJDIR)/des.o
GENERATED += $(OBJDIR)/dhm.o
GENERATED += $(OBJDIR)/ecdh.o
GENERATED += $(OBJDIR)/ecdsa.o
GENERATED += $(OBJDIR)/ecjpake.o
GENERATED += $(OBJDIR)/ecp.o
GENERATED += $(OBJDIR)/ecp_curves.o
GENERATED += $(OBJDIR)/entropy.o
GENERATED += $(OBJDIR)/entropy_poll.o
GENERATED += $(OBJDIR)/error.o
GENERATED += $(OBJDIR)/everest.o
GENERATED += $(OBJDIR)/gcm.o
GENERATED += $(OBJDIR)/havege.o
GENERATED += $(OBJDIR)/hkdf.o
GENERATED += $(OBJDIR)/hmac_drbg.o
GENERATED += $(OBJDIR)/md.o
GENERATED += $(OBJDIR)/md2.o
GENERATED += $(OBJDIR)/md4.o
GENERATED += $(OBJDIR)/md5.o
GENERATED += $(OBJDIR)/memory_buffer_alloc.o
GENERATED += $(OBJDIR)/net_sockets.o
GENERATED += $(OBJDIR)/nist_kw.o
GENERATED += $(OBJDIR)/oid.o
GENERATED += $(OBJDIR)/padlock.o
GENERATED += $(OBJDIR)/pem.o
GENERATED += $(OBJDIR)/pk.o
GENERATED += $(OBJDIR)/pk_wrap.o
GENERATED += $(OBJDIR)/pkcs11.o
GENERATED += $(OBJDIR)/pkcs12.o
GENERATED += $(OBJDIR)/pkcs5.o
GENERATED += $(OBJDIR)/pkparse.o
GENERATED += $(OBJDIR)/pkwrite.o
GENERATED += $(OBJDIR)/platform.o
GENERATED += $(OBJDIR)/platform_util.o
GENERATED += $(OBJDIR)/poly1305.o
GENERATED += $(OBJDIR)/psa_crypto.o
GENERATED += $(OBJDIR)/psa_crypto_driver_wrappers.o
GENERATED += $(OBJDIR)/psa_crypto_se.o
GENERATED += $(OBJDIR)/psa_crypto_slot_management.o
GENERATED += $(OBJDIR)/psa_crypto_storage.o
GENERATED += $(OBJDIR)/psa_its_file.o
GENERATED += $(OBJDIR)/ripemd160.o
GENERATED += $(OBJDIR)/rsa.o
GENERATED += $(OBJDIR)/rsa_internal.o
GENERATED += $(OBJDIR)/sha1.o
GENERATED += $(OBJDIR)/sha256.o
GENERATED += $(OBJDIR)/sha512.o
GENERATED += $(OBJDIR)/ssl_cache.o
GENERATED += $(OBJDIR)/ssl_ciphersuites.o
GENERATED += $(OBJDIR)/ssl_cli.o
GENERATED += $(OBJDIR)/ssl_cookie.o
GENERATED += $(OBJDIR)/ssl_msg.o
GENERATED += $(OBJDIR)/ssl_srv.o
GENERATED += $(OBJDIR)/ssl_ticket.o
GENERATED += $(OBJDIR)/ssl_tls.o
GENERATED += $(OBJDIR)/ssl_tls13_keys.o
GENERATED += $(OBJDIR)/threading.o
GENERATED += $(OBJDIR)/timing.o
GENERATED += $(OBJDIR)/version.o
GENERATED += $(OBJDIR)/version_features.o
GENERATED += $(OBJDIR)/x25519.o
GENERATED += $(OBJDIR)/x509.o
GENERATED += $(OBJDIR)/x509_create.o
GENERATED += $(OBJDIR)/x509_crl.o
GENERATED += $(OBJDIR)/x509_crt.o
GENERATED += $(OBJDIR)/x509_csr.o
GENERATED += $(OBJDIR)/x509write_crt.o
GENERATED += $(OBJDIR)/x509write_csr.o
GENERATED += $(OBJDIR)/xtea.o
OBJECTS += $(OBJDIR)/FStar_UInt128_extracted.o
OBJECTS += $(OBJDIR)/FStar_UInt64_FStar_UInt32_FStar_UInt16_FStar_UInt8.o
OBJECTS += $(OBJDIR)/Hacl_Curve25519.o
OBJECTS += $(OBJDIR)/Hacl_Curve25519_joined.o
OBJECTS += $(OBJDIR)/aes.o
OBJECTS += $(OBJDIR)/aesni.o
OBJECTS += $(OBJDIR)/arc4.o
OBJECTS += $(OBJDIR)/aria.o
OBJECTS += $(OBJDIR)/asn1parse.o
OBJECTS += $(OBJDIR)/asn1write.o
OBJECTS += $(OBJDIR)/base64.o
OBJECTS += $(OBJDIR)/bignum.o
OBJECTS += $(OBJDIR)/blowfish.o
OBJECTS += $(OBJDIR)/camellia.o
OBJECTS += $(OBJDIR)/ccm.o
OBJECTS += $(OBJDIR)/certs.o
OBJECTS += $(OBJDIR)/chacha20.o
OBJECTS += $(OBJDIR)/chachapoly.o
OBJECTS += $(OBJDIR)/cipher.o
OBJECTS += $(OBJDIR)/cipher_wrap.o
OBJECTS += $(OBJDIR)/cmac.o
OBJECTS += $(OBJDIR)/ctr_drbg.o
OBJECTS += $(OBJDIR)/debug.o
OBJECTS += $(OBJDIR)/des.o
OBJECTS += $(OBJDIR)/dhm.o
OBJECTS += $(OBJDIR)/ecdh.o
OBJECTS += $(OBJDIR)/ecdsa.o
OBJECTS += $(OBJDIR)/ecjpake.o
OBJECTS += $(OBJDIR)/ecp.o
OBJECTS += $(OBJDIR)/ecp_curves.o
OBJECTS += $(OBJDIR)/entropy.o
OBJECTS += $(OBJDIR)/entropy_poll.o
OBJECTS += $(OBJDIR)/error.o
OBJECTS += $(OBJDIR)/everest.o
OBJECTS += $(OBJDIR)/gcm.o
OBJECTS += $(OBJDIR)/havege.o
OBJECTS += $(OBJDIR)/hkdf.o
OBJECTS += $(OBJDIR)/hmac_drbg.o
OBJECTS += $(OBJDIR)/md.o
OBJECTS += $(OBJDIR)/md2.o
OBJECTS += $(OBJDIR)/md4.o
OBJECTS += $(OBJDIR)/md5.o
OBJECTS += $(OBJDIR)/memory_buffer_alloc.o
OBJECTS += $(OBJDIR)/net_sockets.o
OBJECTS += $(OBJDIR)/nist_kw.o
OBJECTS += $(OBJDIR)/oid.o
OBJECTS += $(OBJDIR)/padlock.o
OBJECTS += $(OBJDIR)/pem.o
OBJECTS += $(OBJDIR)/pk.o
OBJECTS += $(OBJDIR)/pk_wrap.o
OBJECTS += $(OBJDIR)/pkcs11.o
OBJECTS += $(OBJDIR)/pkcs12.o
OBJECTS += $(OBJDIR)/pkcs5.o
OBJECTS += $(OBJDIR)/pkparse.o
OBJECTS += $(OBJDIR)/pkwrite.o
OBJECTS += $(OBJDIR)/platform.o
OBJECTS += $(OBJDIR)/platform_util.o
OBJECTS += $(OBJDIR)/poly1305.o
OBJECTS += $(OBJDIR)/psa_crypto.o
OBJECTS += $(OBJDIR)/psa_crypto_driver_wrappers.o
OBJECTS += $(OBJDIR)/psa_crypto_se.o
OBJECTS += $(OBJDIR)/psa_crypto_slot_management.o
OBJECTS += $(OBJDIR)/psa_crypto_storage.o
OBJECTS += $(OBJDIR)/psa_its_file.o
OBJECTS += $(OBJDIR)/ripemd160.o
OBJECTS += $(OBJDIR)/rsa.o
OBJECTS += $(OBJDIR)/rsa_internal.o
OBJECTS += $(OBJDIR)/sha1.o
OBJECTS += $(OBJDIR)/sha256.o
OBJECTS += $(OBJDIR)/sha512.o
OBJECTS += $(OBJDIR)/ssl_cache.o
OBJECTS += $(OBJDIR)/ssl_ciphersuites.o
OBJECTS += $(OBJDIR)/ssl_cli.o
OBJECTS += $(OBJDIR)/ssl_cookie.o
OBJECTS += $(OBJDIR)/ssl_msg.o
OBJECTS += $(OBJDIR)/ssl_srv.o
OBJECTS += $(OBJDIR)/ssl_ticket.o
OBJECTS += $(OBJDIR)/ssl_tls.o
OBJECTS += $(OBJDIR)/ssl_tls13_keys.o
OBJECTS += $(OBJDIR)/threading.o
OBJECTS += $(OBJDIR)/timing.o
OBJECTS += $(OBJDIR)/version.o
OBJECTS += $(OBJDIR)/version_features.o
OBJECTS += $(OBJDIR)/x25519.o
OBJECTS += $(OBJDIR)/x509.o
OBJECTS += $(OBJDIR)/x509_create.o
OBJECTS += $(OBJDIR)/x509_crl.o
OBJECTS += $(OBJDIR)/x509_crt.o
OBJECTS += $(OBJDIR)/x509_csr.o
OBJECTS += $(OBJDIR)/x509write_crt.o
OBJECTS += $(OBJDIR)/x509write_csr.o
OBJECTS += $(OBJDIR)/xtea.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking mbedtls
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning mbedtls
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) rmdir /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CC) -x c-header $(ALL_CFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/Hacl_Curve25519_joined.o: 3rdparty/everest/library/Hacl_Curve25519_joined.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/everest.o: 3rdparty/everest/library/everest.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FStar_UInt128_extracted.o: 3rdparty/everest/library/kremlib/FStar_UInt128_extracted.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FStar_UInt64_FStar_UInt32_FStar_UInt16_FStar_UInt8.o: 3rdparty/everest/library/kremlib/FStar_UInt64_FStar_UInt32_FStar_UInt16_FStar_UInt8.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Hacl_Curve25519.o: 3rdparty/everest/library/legacy/Hacl_Curve25519.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x25519.o: 3rdparty/everest/library/x25519.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/aes.o: library/aes.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/aesni.o: library/aesni.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/arc4.o: library/arc4.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/aria.o: library/aria.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/asn1parse.o: library/asn1parse.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/asn1write.o: library/asn1write.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/base64.o: library/base64.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/bignum.o: library/bignum.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/blowfish.o: library/blowfish.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/camellia.o: library/camellia.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ccm.o: library/ccm.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/certs.o: library/certs.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/chacha20.o: library/chacha20.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/chachapoly.o: library/chachapoly.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cipher.o: library/cipher.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cipher_wrap.o: library/cipher_wrap.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/cmac.o: library/cmac.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ctr_drbg.o: library/ctr_drbg.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/debug.o: library/debug.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/des.o: library/des.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/dhm.o: library/dhm.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ecdh.o: library/ecdh.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ecdsa.o: library/ecdsa.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ecjpake.o: library/ecjpake.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ecp.o: library/ecp.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ecp_curves.o: library/ecp_curves.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/entropy.o: library/entropy.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/entropy_poll.o: library/entropy_poll.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/error.o: library/error.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/gcm.o: library/gcm.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/havege.o: library/havege.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/hkdf.o: library/hkdf.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/hmac_drbg.o: library/hmac_drbg.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/md.o: library/md.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/md2.o: library/md2.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/md4.o: library/md4.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/md5.o: library/md5.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/memory_buffer_alloc.o: library/memory_buffer_alloc.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/net_sockets.o: library/net_sockets.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/nist_kw.o: library/nist_kw.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/oid.o: library/oid.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/padlock.o: library/padlock.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pem.o: library/pem.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pk.o: library/pk.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pk_wrap.o: library/pk_wrap.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pkcs11.o: library/pkcs11.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pkcs12.o: library/pkcs12.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pkcs5.o: library/pkcs5.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pkparse.o: library/pkparse.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pkwrite.o: library/pkwrite.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/platform.o: library/platform.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/platform_util.o: library/platform_util.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/poly1305.o: library/poly1305.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/psa_crypto.o: library/psa_crypto.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/psa_crypto_driver_wrappers.o: library/psa_crypto_driver_wrappers.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/psa_crypto_se.o: library/psa_crypto_se.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/psa_crypto_slot_management.o: library/psa_crypto_slot_management.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/psa_crypto_storage.o: library/psa_crypto_storage.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/psa_its_file.o: library/psa_its_file.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ripemd160.o: library/ripemd160.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rsa.o: library/rsa.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/rsa_internal.o: library/rsa_internal.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/sha1.o: library/sha1.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/sha256.o: library/sha256.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/sha512.o: library/sha512.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_cache.o: library/ssl_cache.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_ciphersuites.o: library/ssl_ciphersuites.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_cli.o: library/ssl_cli.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_cookie.o: library/ssl_cookie.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_msg.o: library/ssl_msg.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_srv.o: library/ssl_srv.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_ticket.o: library/ssl_ticket.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_tls.o: library/ssl_tls.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ssl_tls13_keys.o: library/ssl_tls13_keys.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/threading.o: library/threading.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/timing.o: library/timing.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/version.o: library/version.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/version_features.o: library/version_features.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x509.o: library/x509.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x509_create.o: library/x509_create.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x509_crl.o: library/x509_crl.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x509_crt.o: library/x509_crt.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x509_csr.o: library/x509_csr.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x509write_crt.o: library/x509write_crt.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/x509write_csr.o: library/x509write_csr.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/xtea.o: library/xtea.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif
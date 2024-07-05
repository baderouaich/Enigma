#pragma once
#include "Tool.hpp"
#include <Core/Core.hpp>
#include <imgui.h>

#include <infoware/infoware.hpp>


/*
*	System & Hardware Information Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class SystemAndHardwareInfoTool : public Tool {
  public:
    SystemAndHardwareInfoTool() = default;
    ~SystemAndHardwareInfoTool() = default;

  private: /* Helper Functions */
    const char *StringifyArchitectureEnum(const iware::cpu::architecture_t arch) const noexcept {
#define CASE_RET_STR(c)               \
  case iware::cpu::architecture_t::c: \
    return #c
      switch (arch) {
        CASE_RET_STR(x64);
        CASE_RET_STR(arm);
        CASE_RET_STR(itanium);
        CASE_RET_STR(x86);
        default:
          return "<unknown arch>";
      }
#undef CASE_RET_STR
    }

    const char *StringifyCacheTypeEnum(const iware::cpu::cache_type_t cache_type) const noexcept {
#define CASE_RET_STR(c)             \
  case iware::cpu::cache_type_t::c: \
    return #c
      switch (cache_type) {
        CASE_RET_STR(unified);
        CASE_RET_STR(instruction);
        CASE_RET_STR(data);
        CASE_RET_STR(trace);
        default:
          return "<unknown cache type>";
      }
#undef CASE_RET_STR
    }

    const char *StringifyInstructionSetEnum(const iware::cpu::instruction_set_t is) const noexcept {
#define CASE_RET_STR(c)                  \
  case iware::cpu::instruction_set_t::c: \
    return #c
      switch (is) {
        CASE_RET_STR(s3d_now);
        CASE_RET_STR(s3d_now_extended);
        CASE_RET_STR(mmx);
        CASE_RET_STR(mmx_extended);
        CASE_RET_STR(sse);
        CASE_RET_STR(sse2);
        CASE_RET_STR(sse3);
        CASE_RET_STR(ssse3);
        CASE_RET_STR(sse4a);
        CASE_RET_STR(sse41);
        CASE_RET_STR(sse42);
        CASE_RET_STR(aes);
        CASE_RET_STR(avx);
        CASE_RET_STR(avx2);
        CASE_RET_STR(avx_512);
        CASE_RET_STR(avx_512_f);
        CASE_RET_STR(avx_512_cd);
        CASE_RET_STR(avx_512_pf);
        CASE_RET_STR(avx_512_er);
        CASE_RET_STR(avx_512_vl);
        CASE_RET_STR(avx_512_bw);
        CASE_RET_STR(avx_512_bq);
        CASE_RET_STR(avx_512_dq);
        CASE_RET_STR(avx_512_ifma);
        CASE_RET_STR(avx_512_vbmi);
        CASE_RET_STR(hle);
        CASE_RET_STR(bmi1);
        CASE_RET_STR(bmi2);
        CASE_RET_STR(adx);
        CASE_RET_STR(mpx);
        CASE_RET_STR(sha);
        CASE_RET_STR(prefetch_wt1);
        CASE_RET_STR(fma3);
        CASE_RET_STR(fma4);
        CASE_RET_STR(xop);
        CASE_RET_STR(rd_rand);
        CASE_RET_STR(x64);
        CASE_RET_STR(x87_fpu);
        default:
          return "<unknown instruction set>";
      }
#undef CASE_RET_STR
    }

    const char *StringifyKernelVariantEnum(const iware::system::kernel_t variant) const noexcept {
      switch (variant) {
        case iware::system::kernel_t::windows_nt:
          return "Windows NT";
        case iware::system::kernel_t::linux:
          return "Linux";
        case iware::system::kernel_t::darwin:
          return "Darwin";
        case iware::system::kernel_t::unknown:
        default:
          return "<unknown kernel variant>";
      }
    }

    const char *StringifyGPUVendorEnum(const iware::gpu::vendor_t vendor) const noexcept {
      switch (vendor) {
        case iware::gpu::vendor_t::intel:
          return "Intel";
        case iware::gpu::vendor_t::amd:
          return "AMD";
        case iware::gpu::vendor_t::nvidia:
          return "Nvidia";
        case iware::gpu::vendor_t::microsoft:
          return "Microsoft";
        case iware::gpu::vendor_t::qualcomm:
          return "Qualcomm";
        case iware::gpu::vendor_t::apple:
          return "Apple";
        case iware::gpu::vendor_t::unknown:
        default:
          return "<unknown gpu vendor>";
      }
    }

  public: /* Tool Life Cycle */
    void OnCreate() override;
    void OnDraw(Scene *parent) override;
    void OnDestroy() override;
};
NS_ENIGMA_END

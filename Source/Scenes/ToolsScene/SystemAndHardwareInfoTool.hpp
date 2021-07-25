#pragma once
#include "Tool.hpp"
#include <Core/Core.hpp>
#include <imgui.h>

#include <infoware/infoware.hpp>


/*
*	System Information Tool Collapsing Header View To be drawn in ToolsScene
*/
NS_ENIGMA_BEGIN
class SystemAndHardwareInfoTool : public Tool
{
	struct CPUInformation
	{
		CPUInformation()
		{
			quantities = iware::cpu::quantities();
			for (i32 i = 0; i < caches.size(); ++i)
			{
				caches[i] = iware::cpu::cache(i + 1);
			}
			architecture = GetArchitectureStr(iware::cpu::architecture());
			frequency = iware::cpu::frequency();
			endianness = iware::cpu::endianness() == iware::cpu::endianness_t::big ? "Big" : "Little";
			model_name = iware::cpu::model_name();
			vendor_id = iware::cpu::vendor_id();

			for (const auto is : iware::cpu::supported_instruction_sets())
			{
				supported_instructions_set.push_back(GetInstructionSetStr(is));
			}
		}
		~CPUInformation()
		{
		}

		String GetArchitectureStr(const iware::cpu::architecture_t arch)
		{
#define CASE_RET_STR(c) case iware::cpu::architecture_t::c: return String(#c)
			switch (arch)
			{
				CASE_RET_STR(x64);
				CASE_RET_STR(arm);
				CASE_RET_STR(itanium);
				CASE_RET_STR(x86);
				CASE_RET_STR(unknown);
				default: return String("<unknown arch>");
			}
#undef CASE_RET_STR
		}

		String GetCacheTypeStr(const iware::cpu::cache_type_t cache_type) noexcept
		{
#define CASE_RET_STR(c) case iware::cpu::cache_type_t::c: return String(#c)
			switch (cache_type)
			{
				CASE_RET_STR(unified);
				CASE_RET_STR(instruction);
				CASE_RET_STR(data);
				CASE_RET_STR(trace);
				default: return String("<unknown cache type>");
			}
#undef CASE_RET_STR
		}
		
		String GetInstructionSetStr(const iware::cpu::instruction_set_t is) noexcept
		{
#define CASE_RET_STR(c) case iware::cpu::instruction_set_t::c: return String(#c)
			switch (is)
			{
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
				default: return String("<unknown instruction set>");
			}
#undef CASE_RET_STR
		}

		iware::cpu::quantities_t quantities{};
		std::array<iware::cpu::cache_t, 3> caches{};
		String architecture{};
		ui64 frequency{};
		String endianness;
		String model_name{};
		String vendor_id{};
		std::vector<String> supported_instructions_set{}; // SSE, SSE2, AVX...

	};


public:
	SystemAndHardwareInfoTool() = default;
	~SystemAndHardwareInfoTool() = default;


private:
	std::unique_ptr<CPUInformation> m_cpu_info{};

public: /* Tool Life Cycle */
	void OnCreate() override;
	void OnDraw(Scene* parent) override;
	void OnDestroy() override;
};
NS_ENIGMA_END

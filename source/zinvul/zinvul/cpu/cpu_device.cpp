///*!
//  \file cpu_device.cpp
//  \author Sho Ikeda
//  \brief No brief description
//
//  \details
//  No detailed description.
//
//  \copyright
//  Copyright (c) 2015-2020 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#include "cpu_device.hpp"
//// Standard C++ library
//#include <string>
//// cpu_features
//#include "cpu_features_macros.h"
//#if defined(CPU_FEATURES_ARCH_X86)
//#include "cpuinfo_x86.h"
//#elif defined(CPU_FEATURES_ARCH_ARM)
//#include "cpuinfo_arm.h"
//#elif defined(CPU_FEATURES_ARCH_AARCH64)
//#include "cpuinfo_aarch64.h"
//#elif defined(CPU_FEATURES_ARCH_MIPS)
//#include "cpuinfo_mips.h"
//#elif defined(CPU_FEATURES_ARCH_PPC)
//#include "cpuinfo_ppc.h"
//#endif
//// Zisc
//#include "zisc/std_memory_resource.hpp"
//// Zinvul
//#include "zinvul/device.hpp"
//#include "zinvul/device_options.hpp"
//
//namespace zinvul {
//
///*!
//  \details No detailed description
//  */
//void CpuDevice::initCpuInfo() noexcept
//{
//  using namespace cpu_features;
//  // Initialize device info
//#if defined(CPU_FEATURES_ARCH_X86)
//  {
//    char brand_string[49];
//    FillX86BrandString(brand_string);
//    name_ = brand_string;
//  }
//  {
//    const X86Info info = GetX86Info();
//    vendor_name_ = info.vendor;
//  }
//#elif defined(CPU_FEATURES_ARCH_ARM)
//  static_assert(false, "Not implemented yet.");
//  {
//    const ArmInfo info = GetArmInfo();
//  }
//  vendor_name_ = "ARM";
//#elif defined(CPU_FEATURES_ARCH_AARCH64)
//  static_assert(false, "Not implemented yet.");
//  {
//    const Aarch64Info info = GetAarch64Info();
//  }
//#elif defined(CPU_FEATURES_ARCH_MIPS)
//  static_assert(false, "Not implemented yet.");
//  {
//    const MipsInfo info = GetMipsInfo();
//  }
//#elif defined(CPU_FEATURES_ARCH_PPC)
//  static_assert(false, "Not implemented yet.");
//  {
//    const PPCInfo info = GetPPCInfo();
//    const PPCPlatformStrings strings = GetPPCPlatformStrings();
//  }
//#endif
//  if (name_.empty())
//    name_ = "N/A";
//  if (vendor_name_.empty())
//    vendor_name_ = "N/A";
//}
//
///*!
//  \details No detailed description
//
//  \param [in] options No description.
//  */
//void CpuDevice::initialize(DeviceOptions& /* options */) noexcept
//{
//  initCpuInfo();
//}
//
//} // namespace zinvul

/*
 * cmsis_version.h
 *
 *  Created on: Oct 12, 2025
 *      Author: varun.s.patil
 */

/**
 ******************************************************************************
 * @file    cmsis_version.h
 * @brief   CMSIS Core(M) Version definitions
 *
 * @details
 * Defines version information for the CMSIS Core(M) component.
 * This header provides macros indicating the major and sub-version
 * of the CMSIS Core library, allowing conditional compilation and
 * version-based compatibility checks across CMSIS modules.
 *
 * ----------------------------------------------------------------------------
 * CONTENT OVERVIEW
 * ----------------------------------------------------------------------------
 * 1. Version Macros
 *    - __CM_CMSIS_VERSION_MAIN  : Main (major) version number
 *    - __CM_CMSIS_VERSION_SUB   : Sub (minor) version number
 *    - __CM_CMSIS_VERSION       : Combined 32-bit version value
 *
 * ----------------------------------------------------------------------------
 * PURPOSE
 * ----------------------------------------------------------------------------
 * - Identifies the CMSIS Core(M) release used in the build.
 * - Enables version-based feature control and validation.
 *
 * ----------------------------------------------------------------------------
 * COMPATIBILITY
 * ----------------------------------------------------------------------------
 * - Target: ARM Cortex-M processors
 * - CMSIS Core Version: 5.6
 * - Included automatically by CMSIS core headers (e.g., core_cm4.h)
 *
 * ----------------------------------------------------------------------------
 * COPYRIGHT NOTICE
 * ----------------------------------------------------------------------------
 * Copyright (c) ARM Limited.
 * Distributed under the Apache 2.0 License.
 ******************************************************************************
 */

#if   defined ( __ICCARM__ )
  #pragma system_include         /* treat file as system include file for MISRA check */
#elif defined (__clang__)
  #pragma clang system_header   /* treat file as system include file */
#endif

#ifndef CMSIS_VERSION_H_
#define CMSIS_VERSION_H_

/*  CMSIS Version definitions */
#define __CM_CMSIS_VERSION_MAIN  ( 5U)                                      /*!< [31:16] CMSIS Core(M) main version */
#define __CM_CMSIS_VERSION_SUB   ( 6U)                                      /*!< [15:0]  CMSIS Core(M) sub version */
#define __CM_CMSIS_VERSION       ((__CM_CMSIS_VERSION_MAIN << 16U) | \
                                   __CM_CMSIS_VERSION_SUB           )       /*!< CMSIS Core(M) version number */

#endif /* CMSIS_VERSION_H_ */

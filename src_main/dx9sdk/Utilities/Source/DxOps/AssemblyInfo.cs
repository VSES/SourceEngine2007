//--------------------------------------------------------------------------------------
// File: AssemblyInfo.cs
//
// Assembly information for all managed samples
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

using System;
using System.Security;
using System.Security.Permissions;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

// Set assembly information
[assembly: AssemblyTitle("Microsoft DirectX SDK Tools - DXOPS")]
[assembly: AssemblyDescription("DXOPS for the Microsoft DirectX SDK")]
[assembly: AssemblyCompany("Microsoft")]
[assembly: AssemblyProduct("DirectX SDK")]
[assembly: AssemblyCopyright("Copyright (c) Microsoft Corporation. All rights reserved.")]
// Update version
[assembly: AssemblyVersion("9.18.944.0000")]

// We will use UInt which isn't CLS compliant, possible unsafe code as well
[assembly: CLSCompliant(false)]
[assembly: ComVisible(false)]

// Security information
[assembly: SecurityPermissionAttribute(SecurityAction.RequestMinimum, UnmanagedCode=true)]
[assembly: SecurityPermissionAttribute(SecurityAction.RequestMinimum, Execution=true)]

// We want to be able to read the registry key
[assembly: RegistryPermissionAttribute(SecurityAction.RequestMinimum,Read="HKEY_LOCAL_MACHINE\\Software\\Microsoft\\DirectX SDK")]

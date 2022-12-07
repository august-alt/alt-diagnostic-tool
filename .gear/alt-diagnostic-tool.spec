%define _unpackaged_files_terminate_build 1

Name: alt-diagnostic-tool
Version: 0.1.0
Release: alt1

Summary: ALT Diagnostic tool
License: GPLv2+
Group: Other
Url: https://github.com/august-alt/alt-diagnostic-tool

BuildRequires: cmake
BuildRequires: rpm-macros-cmake
BuildRequires: cmake-modules
BuildRequires: gcc-c++
BuildRequires: qt5-base-devel
BuildRequires: qt5-declarative-devel
BuildRequires: qt5-tools-devel
BuildRequires: qt5-base-common
BuildRequires: doxygen

Source0: %name-%version.tar

%description
ALT Linux diagnostic tool template.

%prep
%setup -q

%build
%cmake
%cmake_build

%install
%cmakeinstall_std

%files
%doc README.md
%doc INSTALL.md

%changelog
* Wed Dec 07 2022 Vladimir Rubanov <august@altlinux.org> 0.1.0-alt1
- Initial build

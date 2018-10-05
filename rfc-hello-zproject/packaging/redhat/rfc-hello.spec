#
#    rfc-hello - Project
#
#    Copyright (c) the Authors
#

# To build with draft APIs, use "--with drafts" in rpmbuild for local builds or add
#   Macros:
#   %_with_drafts 1
# at the BOTTOM of the OBS prjconf
%bcond_with drafts
%if %{with drafts}
%define DRAFTS yes
%else
%define DRAFTS no
%endif
Name:           rfc-hello
Version:        0.0.0
Release:        1
Summary:        project
License:        MIT
URL:            http://example.com/
Source0:        %{name}-%{version}.tar.gz
Group:          System/Libraries
# Note: ghostscript is required by graphviz which is required by
#       asciidoc. On Fedora 24 the ghostscript dependencies cannot
#       be resolved automatically. Thus add working dependency here!
BuildRequires:  ghostscript
BuildRequires:  asciidoc
BuildRequires:  automake
BuildRequires:  autoconf
BuildRequires:  libtool
BuildRequires:  pkgconfig
BuildRequires:  xmlto
BuildRequires:  zeromq-devel
BuildRequires:  czmq-devel
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
rfc-hello project.

%package -n librfc_hello0
Group:          System/Libraries
Summary:        project shared library

%description -n librfc_hello0
This package contains shared library for rfc-hello: project

%post -n librfc_hello0 -p /sbin/ldconfig
%postun -n librfc_hello0 -p /sbin/ldconfig

%files -n librfc_hello0
%defattr(-,root,root)
%{_libdir}/librfc_hello.so.*

%package devel
Summary:        project
Group:          System/Libraries
Requires:       librfc_hello0 = %{version}
Requires:       zeromq-devel
Requires:       czmq-devel

%description devel
project development tools
This package contains development files for rfc-hello: project

%files devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/librfc_hello.so
%{_libdir}/pkgconfig/librfc_hello.pc
%{_mandir}/man3/*
%{_mandir}/man7/*

%prep

%setup -q

%build
sh autogen.sh
%{configure} --enable-drafts=%{DRAFTS}
make %{_smp_mflags}

%install
make install DESTDIR=%{buildroot} %{?_smp_mflags}

# remove static libraries
find %{buildroot} -name '*.a' | xargs rm -f
find %{buildroot} -name '*.la' | xargs rm -f

%files
%defattr(-,root,root)
%{_bindir}/rfc-hello-user
%{_mandir}/man1/rfc-hello-user*
%{_bindir}/provider
%{_mandir}/man1/provider*
%{_bindir}/justforsake
%{_mandir}/man1/justforsake*

%changelog

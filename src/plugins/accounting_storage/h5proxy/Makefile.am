# Makefile for accounting_storage/h5proxy plugin

AUTOMAKE_OPTIONS = foreign

PLUGIN_FLAGS = -module -avoid-version --export-dynamic

AM_CXXFLAGS = -DSLURM_PLUGIN_DEBUG -I$(top_srcdir) -I$(top_srcdir)/src/common

pkglib_LTLIBRARIES = accounting_storage_h5proxy.la

AS_H5PROXY_SOURCES =  implemented.c accounting_storage_h5proxy.c

accounting_storage_h5proxy_la_SOURCES = $(AS_H5PROXY_SOURCES)
accounting_storage_h5proxy_la_LDFLAGS = $(PLUGIN_FLAGS)

set(ProjectName "libprotobuf")
IF( WIN32 )
	
	message("win32_protobuf")
	include_directories(Include)
	include_directories(./vsprojects)
	set(pbFiles
		./src/google/protobuf/io/coded_stream.cc
		./src/google/protobuf/stubs/common.cc
		./src/google/protobuf/descriptor.cc
		./src/google/protobuf/descriptor.pb.cc
		./src/google/protobuf/descriptor_database.cc
		./src/google/protobuf/dynamic_message.cc
		./src/google/protobuf/extension_set.cc
		./src/google/protobuf/extension_set_heavy.cc
		./src/google/protobuf/generated_message_reflection.cc
		./src/google/protobuf/generated_message_util.cc
		./src/google/protobuf/io/gzip_stream.cc
		./src/google/protobuf/compiler/importer.cc
		./src/google/protobuf/message.cc
		./src/google/protobuf/message_lite.cc
		./src/google/protobuf/stubs/once.cc
		./src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc
		./src/google/protobuf/compiler/parser.cc
		./src/google/protobuf/io/printer.cc
		./src/google/protobuf/reflection_ops.cc
		./src/google/protobuf/repeated_field.cc
		./src/google/protobuf/service.cc
		./src/google/protobuf/stubs/structurally_valid.cc
		./src/google/protobuf/stubs/strutil.cc
		./src/google/protobuf/stubs/substitute.cc
		./src/google/protobuf/text_format.cc
		./src/google/protobuf/io/tokenizer.cc
		./src/google/protobuf/unknown_field_set.cc
		./src/google/protobuf/wire_format.cc
		./src/google/protobuf/wire_format_lite.cc
		./src/google/protobuf/io/zero_copy_stream.cc
		./src/google/protobuf/io/zero_copy_stream_impl.cc
		./src/google/protobuf/io/zero_copy_stream_impl_lite.cc
		./src/google/protobuf/stubs/stringprintf.cc)
	add_library(${ProjectName} SHARED ${pbFiles})
	set_target_properties( ${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG "${ProjectName}_d")
	set_target_properties( ${ProjectName} PROPERTIES PREFIX "")
	set_target_properties( ${ProjectName} PROPERTIES 
		FOLDER "NFDeps"
		ARCHIVE_OUTPUT_DIRECTORY ${NFOutPutDir}
		RUNTIME_OUTPUT_DIRECTORY ${NFOutPutDir}
		LIBRARY_OUTPUT_DIRECTORY ${NFOutPutDir} )
	add_definitions(
		-D_USRDLL
		-DLIBPROTOBUF_EXPORTS
		-DPROTOBUF_USE_DLLS
		-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
		-D_SCL_SECURE_NO_WARNINGS
		)
ELSE()
	message("not win32")
ENDIF()


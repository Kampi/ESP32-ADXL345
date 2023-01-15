set(COMPONENT_SRCS
    "src/adxl345.cpp"
    )

set(COMPONENT_ADD_INCLUDEDIRS
	"include"
	)

set(COMPONENT_PRIV_REQUIRES freertos)

register_component()
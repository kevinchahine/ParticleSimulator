set (APP_NAME ParticleSimulator)

set (CONFIG
	"config/force_constants_01.yaml"
)

set (CLOUD
	"source/cloud/cloud.hpp"
	"source/cloud/acceleration_cloud.h"
	"source/cloud/acceleration_cloud.cpp"
	"source/cloud/charge_cloud.cpp"
	"source/cloud/charge_cloud.h"
	"source/cloud/cloud.cpp"
	"source/cloud/cloud.h"
	"source/cloud/cloud_template.h"
	"source/cloud/cloud_template_1.h"
	"source/cloud/cloud_template_2.h"
	"source/cloud/cloud_template_square.h"
	"source/cloud/distance_cloud.cpp"
	"source/cloud/distance_cloud.h"
	"source/cloud/element_cloud.cpp"
	"source/cloud/element_cloud.h"
	"source/cloud/force_cloud.cpp"
	"source/cloud/force_cloud.h"
	"source/cloud/mass_cloud.cpp"
	"source/cloud/mass_cloud.h"
	"source/cloud/position_cloud.cpp"
	"source/cloud/position_cloud.h"
	"source/cloud/velocity_cloud.cpp"
	"source/cloud/velocity_cloud.h"
)

set (SOURCE
	"source/ui/display.cpp"
	"source/ui/display.h"
	"source/factory/distribution.cpp"
	"source/factory/distribution.h"
	"source/factory/factory.cpp"
	"source/factory/factory.h"
	"source/factory/factory_options.cpp"
	"source/factory/factory_options.h"
	"source/constants.cpp"
	"source/constants.h"
	"source/force_engine.cpp"
	"source/force_engine.h"
	"source/globals.cpp"
	"source/globals.h"
	"source/main.cpp"
	"source/particle.cpp"
	"source/particle.h"
	"source/particle_type.cpp"
	"source/particle_type.h"
	"source/stop_watch.cpp" 
	"source/stop_watch.h"
)

set (UI
	
)

set (NOTES
	notes/01_topic.txt
	notes/02_model_view_controller.txt
	notes/03_acceleration.txt
)

add_executable (${APP_NAME}
	${CONFIG}
	${CLOUD}
	${SOURCE}
	${UI}
	${NOTES}
)

include_directories(".")

message(STATUS "--- OpenCV ---")
find_package(OpenCV REQUIRED)
#message(STATUS "OpenCV_INCLUDE_DIRS = OpenCV_INCLUDE_DIRS")
#message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
target_link_libraries(${APP_NAME} PUBLIC ${OpenCV_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ParticleSimulator PROPERTY CXX_STANDARD 20)
endif()


#include <include/ecs_nested_prefab.h>

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    ECS_IMPORT(world, FlecsComponentsTransform, ECS_2D);
    ECS_IMPORT(world, FlecsComponentsGeometry, ECS_2D);
    ECS_IMPORT(world, FlecsComponentsGraphics, ECS_2D);
    ECS_IMPORT(world, FlecsComponentsInput, ECS_2D);
    ECS_IMPORT(world, FlecsSystemsSdl2, ECS_2D);

    /* Top level prefab that describes a wheel and tire. This will serve as the
     * base for the front and back wheels of the car. */
    ECS_ENTITY(world, WheelPrefab, EcsPrefab, EcsCircle, EcsColor);
        ecs_set(world, WheelPrefab, EcsCircle, {.radius = 12});
        ecs_set(world, WheelPrefab, EcsColor, {70, 70, 70, 255});

        ECS_ENTITY(world, WheelChild, EcsPrefab, EcsPosition2D, EcsCircle, EcsColor);
            ecs_set(world, WheelChild, EcsPrefab, {.parent = WheelPrefab});
            ecs_set(world, WheelChild, EcsPosition2D, {0, 0});
            ecs_set(world, WheelChild, EcsCircle, {.radius = 6});
            ecs_set(world, WheelChild, EcsColor, {200, 200, 200, 255});

    /* Top level prefab that describes a car */
    ECS_ENTITY(world, CarPrefab, EcsPrefab);
        ECS_ENTITY(world, Chassis, EcsPrefab, EcsPosition2D, EcsColor);
            ecs_set(world, Chassis, EcsPrefab, {.parent = CarPrefab});
            ecs_set(world, Chassis, EcsPosition2D, {0, 0});
            ecs_set(world, Chassis, EcsRectangle, {.width = 100, .height = 25});
            ecs_set(world, Chassis, EcsColor, {255, 0, 0, 255});

            ECS_ENTITY(world, ChassisTop, EcsPrefab, EcsPosition2D, EcsColor);
                ecs_set(world, ChassisTop, EcsPrefab, {.parent = Chassis});
                ecs_set(world, ChassisTop, EcsPosition2D, {-5, -15});
                ecs_set(world, ChassisTop, EcsRectangle, {.width = 70, .height = 25});
                ecs_set(world, ChassisTop, EcsColor, {255, 0, 0, 255});

        /* Add wheels to the car. The WheelFront and WheelBack prefabs are
         * children of CarPrefab, and have WheelPrefab as base. This will also
         * add the children of the WheelPrefab base to the WheelFront and 
         * WheelBack prefabs. */
        ECS_ENTITY(world, WheelFront, EcsPrefab, WheelPrefab, EcsPosition2D);
            ecs_set(world, WheelFront, EcsPrefab, {.parent = CarPrefab});
            ecs_set(world, WheelFront, EcsPosition2D, {30, 15});

        ECS_ENTITY(world, WheelBack, EcsPrefab, WheelPrefab, EcsPosition2D);
            ecs_set(world, WheelBack, EcsPrefab, {.parent = CarPrefab});
            ecs_set(world, WheelBack, EcsPosition2D, {-30, 15});

    /* Instantiate three cars. This will instantiate the entire prefab hierarchy
     * for each created entity. */
    ecs_entity_t e = ecs_new(world, CarPrefab);
    ecs_set(world, e, EcsPosition2D, {0, 0});

    e = ecs_new(world, CarPrefab);
    ecs_set(world, e, EcsPosition2D, {-150, 0});

    e = ecs_new(world, CarPrefab);
    ecs_set(world, e, EcsPosition2D, {150, 0});

    /* Create a few loose tires */
    e = ecs_new(world, WheelPrefab);
    ecs_set(world, e, EcsPosition2D, {300, 0});

    e = ecs_new(world, WheelPrefab);
    ecs_set(world, e, EcsPosition2D, {300, -150});
    
    e = ecs_new(world, WheelPrefab);
    ecs_set(world, e, EcsPosition2D, {300, 150});

    /* Initialize canvas */
    ecs_set(world, 0, EcsCanvas2D, {
        .window = { .width = 800, .height = 600 }, .title = "Hello ecs_nested_prefab!" 
    });

    /* Enter main loop */
    ecs_set_target_fps(world, 60);

    while ( ecs_progress(world, 0));

    /* Cleanup */
    return ecs_fini(world);
}

@rem Enhancement parameters

rem Default
set colortuning_cbexp=1.0
set colortuning_crexp=1.0
set medfilt_radius1=0
set medfilt_radius2=0
set medfilt_radius3=0
set unsharpmask_radius1=0.1
set unsharpmask_amount1=0.0
set unsharpmask_radius2=0.1
set unsharpmask_amount2=0.0
set unsharpmask_radius3=0.1
set unsharpmask_amount3=0.0

rem Original
if %enhancement_mode%==0_0 (
    set colortuning_enabled=false
    set medfilt_enabled=false
    set unsharpmask_enabled=false
)

rem Weak filtering
if %enhancement_mode%==0_1 (
    set medfilt_radius1=2
    set medfilt_radius2=1
    set medfilt_radius3=1

    set unsharpmask_radius1=2.0
    set unsharpmask_amount1=0.5
    set unsharpmask_radius2=2.0
    set unsharpmask_amount2=0.5
    set unsharpmask_radius3=2.0
    set unsharpmask_amount3=0.5

    set colortuning_enabled=false
    set medfilt_enabled=true
    set unsharpmask_enabled=true
)

rem Strong filtering
if %enhancement_mode%==0_2 (
    set medfilt_radius1=3
    set medfilt_radius2=2
    set medfilt_radius3=2

    set unsharpmask_radius1=2.0
    set unsharpmask_amount1=0.5
    set unsharpmask_radius2=2.0
    set unsharpmask_amount2=0.5
    set unsharpmask_radius3=2.0
    set unsharpmask_amount3=0.5

    set colortuning_enabled=false
    set medfilt_enabled=true
    set unsharpmask_enabled=true
)

rem Color correction
if %enhancement_mode%==1_0 (
    set colortuning_cbexp=0.8
    set colortuning_crexp=0.8

    set colortuning_enabled=true
    set medfilt_enabled=false
    set unsharpmask_enabled=false
)

rem Color correction and weak filtering
if %enhancement_mode%==1_1 (
    set colortuning_cbexp=0.8
    set colortuning_crexp=0.8

    set medfilt_radius1=2
    set medfilt_radius2=1
    set medfilt_radius3=1

    set unsharpmask_radius1=2.0
    set unsharpmask_amount1=0.5
    set unsharpmask_radius2=2.0
    set unsharpmask_amount2=0.5
    set unsharpmask_radius3=2.0
    set unsharpmask_amount3=0.5

    set colortuning_enabled=true
    set medfilt_enabled=true
    set unsharpmask_enabled=true
)

rem Color correction and strong filtering
if %enhancement_mode%==1_2 (
    set colortuning_cbexp=0.8
    set colortuning_crexp=0.8

    set medfilt_radius1=3
    set medfilt_radius2=2
    set medfilt_radius3=2

    set unsharpmask_radius1=2.0
    set unsharpmask_amount1=0.5
    set unsharpmask_radius2=2.0
    set unsharpmask_amount2=0.5
    set unsharpmask_radius3=2.0
    set unsharpmask_amount3=0.5

    set colortuning_enabled=true
    set medfilt_enabled=true
    set unsharpmask_enabled=true
)

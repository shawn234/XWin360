XWin360
=======

Desktop Control for XInput Controllers, such as the XBox 360 Controller


CONTROLS (so far):

Navigation Mode:

    Right Analog Stick - Mouse Movement
    Left Analog Stick - Scroll (Horizontal/Vertical)
    D-pad - Arrow Keys
    Left Bumper/Right Bumper - Alt+TAB (and ALT+SHIFT+TAB) (TODO: not working in Windows 8 due to changes in Accessibility Permissions. Should fix)
    A - Enter
    B - ESC
    X - Right Click
    Y - Switch to Typing Mode
    Left Trigger - Throttle Mouse Speed (The farther the Left Trigger is held down, the slower the cursor moves)
    Right Trigger - Left Click (Supports click-and-drag)

Typing Mode:

    Left Analog Stick - Select typing sector in DaisyWheel UI
        Left Analog Stick Direction + A/B/X/Y - Type corresponding character in highlighted sector in DaisyWheel UI
    Left Bumper - Backspace (currently does not support hold for sustained backspace, not sure why)
    Right Bumper - Space (like Backspace, currently does not support sustained space)
    A (with no direction in Left Analog Stick) - ENTER and exit Typing Mode to Navigation Mode
    B (with no direction in Left Analog Stick) - exit Typing Mode to Navigation Mode

Mode Neutral:

    D-Pad - Arrow keys
    Right Analog Stick - Mouse
    Right Trigger - Left Click
    Start - Windows Key
    Select - Terminates Program


BUGS:
    - Scrolling sometimes causes monitor to shut off and on. Happens in some other cases too
TODO:
    - Implement Alt + TAB (and ALT+SHIFT+TAB) permissions
    - Fix Scrolling Bug
    - Typing mode currently only supports lowercase, and does not have a mapping for periods. Should look to Steam Big Picture Mode DaisyWheel UI for mappings
    - Black corners on Typing Mode UI is not transparent. Figure out how to make black transparent and make the rest of the image transparent as well.
    - Sometimes DaisyWheel UI does not draw on top of all windows. Find some way to draw on top and keep on top of all other windows
    - System Tray Icon
    - DaisyWheel UI is always full resolution, which takes up a lot of space. Find way to scale based on desktop resolution.

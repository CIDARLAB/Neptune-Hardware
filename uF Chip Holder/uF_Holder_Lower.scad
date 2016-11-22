uF_w = 47.4;
uF_l = 30.5;
uF_h = 5.5;
holder_edge_from_uF_edge = 15;     // Must be > 15
holder_overhang_into_uF_edge = 5;



h = 8;
l = 85.5;
w = 127.5;
screw_hole_r = 4/2;
screw_from_edge = 22;

    difference() {
    cube([w, l, h+uF_h]);
    /*    Don't need because chip will sit flush with this top piece
       // Lower sides
        translate([0,l/2,h+uF_h])
            cube([holder_edge_from_uF_edge*2,l+1,uF_h*2], center = true);
        
        translate([w,l/2,h+uF_h])
            cube([holder_edge_from_uF_edge*2,l+1,uF_h*2], center = true);
        
        translate([w/2,0,h+uF_h])
            cube([w+1,holder_edge_from_uF_edge*2,uF_h*2], center = true);
        
        translate([w/2,l,h+uF_h])
            cube([w+1,holder_edge_from_uF_edge*2,uF_h*2], center = true);
        */
        
       // uF Hole
       translate([w/2, l/2, h/2]) 
        cube([uF_w-holder_overhang_into_uF_edge*2, uF_l-holder_overhang_into_uF_edge*2, 500], center = true);
        
       // Bottom Left hole
       translate([screw_from_edge+12, screw_from_edge, 0]) 
    cylinder(h = 50, r = screw_hole_r);
        
       // Bottom Right hole
       translate([w-screw_from_edge-12, screw_from_edge, 0]) 
    cylinder(h = 50, r = screw_hole_r);
        
       // Top Left hole
       translate([screw_from_edge+12, l-screw_from_edge, 0]) 
    cylinder(h = 50, r = screw_hole_r);
        
       // Top Right hole
       translate([w-screw_from_edge-12, l-screw_from_edge, 0]) 
    cylinder(h = 50, r = screw_hole_r);
        
       // Bottom Middle hole
       translate([w/2, screw_from_edge, 0]) 
    cylinder(h = 50, r = screw_hole_r);

       // Top Middle hole
       translate([w/2, l-screw_from_edge, 0]) 
    cylinder(h = 50, r = screw_hole_r);
        
       // Left Middle hole
       translate([screw_from_edge+12, l/2, 0]) 
    cylinder(h = 50, r = screw_hole_r);

       // Right Middle hole
       translate([w-screw_from_edge-12, l/2, 0]) 
    cylinder(h = 50, r = screw_hole_r);
    }

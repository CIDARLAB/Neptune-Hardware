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
cube([w, l, h+uF_h]);       // hight to account for uF support edge
    
   // uF Support Edge
   translate([w/2, l/2, h+uF_h])    
    cube([uF_w, uF_l, 2*uF_h], center = true);   
    
   // uF Hole
   translate([w/2, l/2, (h+uF_h)/2]) 
    cube([uF_w-holder_overhang_into_uF_edge*2, uF_l-holder_overhang_into_uF_edge*2, 100], center = true);
    
   // Holes for valves to escape
        // the +1 is for clerance
        // /5 is arbitrary
   translate([w/4, l/2, 0]) 
    cube([w/5,l+1,5*2], center = true); 
    
   translate([3*w/4, l/2, 0]) 
    cube([w/5,l+1,5*2], center = true); 
   
   translate([w/2, l/4, 0]) 
    cube([w+1,l/5,5*2], center = true);
    
   translate([w/2, 3*l/4, 0]) 
    cube([w+1,l/5,5*2], center = true);
    
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

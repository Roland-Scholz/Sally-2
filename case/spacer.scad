$fn = 20;

spacer();


module spacer() {
    rotate([0, -90, 0]) {
        
    difference(){
        cube([10, 10, 15]);
        translate([5, 5, 0])
            cylinder(h=15, r=1.6, center=false);
/*        
        translate([5, 10, 7.5])
            rotate([90, 0, 0])
                cylinder(h=10, r=1.45, center=false);
        
        translate([0, 5, 7.5])
            rotate([90, 0, 90])
                cylinder(h=10, r=1.45, center=false);
*/
    
    };
    spacer_round();
}

module spacer_round() {
    translate([10, 5, 7.5])
    rotate([0, 90, 0])
    difference(){
        cylinder(h=15, r=5);
        cylinder(h=15, r=2);
    };
}

}
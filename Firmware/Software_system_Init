/** * Created by rebeccawolf on 7/28/16.
 */


function deg2rad(degrees){
    var pi = Math.PI;
    return (degrees * (pi/180));

}

function displacement(thetaX, r, b, d, a) {
    return ( r*Math.cos( deg2rad(thetaX) ) ) + Math.sqrt( Math.pow(b, 2) - ( Math.pow((r*Math.sin(deg2rad(thetaX))+d), 2) ) );
}


function initializeSetup(mLmin, r, b, d, a) {

    // what is thetaX: 1st deriv of displacement
    var thetaX = 0;
    var firstDerivativeDisplacement = (-(r*Math.cos(deg2rad(thetaX))*(d+r*Math.sin(deg2rad(thetaX))))/Math.sqrt((Math.pow(b, 2))-(Math.pow((d+r*Math.sin(deg2rad(thetaX))), 2))))-r*Math.sin(deg2rad(thetaX));
    



    var thetaXArray = [];
    var displacementArray = [];
    var increment = 100;
    var stepSize = 360/increment;

    for( var i = 0; i <= increment; i++){
        thetaX_i = -90+stepSize*i;
        thetaXArray.push(thetaX_i);
        displacementArray.push(displacement(thetaX_i, r, b, d, a));
    }
    console.log(displacementArray);
    
    
    



    var myArray = [10, 20, 20.34, 0, -36, 0.23];
    console.log(Math.max(...myArray));

    //calculate variables we need
    theta_min = brentq(first_derivitive_displacement,-90,75)
    theta_max = brentq(first_derivitive_displacement,75,270)
    Xmin = displacement(theta_min)
    Xmax = displacement(theta_max)
    mLmax = mLmin - (Xmin-Xmax)/a

    return (theta_min,theta_max,Xmin,Xmax,mLmax,mLmin,r,b,d,a)

}
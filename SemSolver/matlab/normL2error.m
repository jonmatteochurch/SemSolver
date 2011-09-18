function err = normL2error( x, y, u, xex, yex, uex )

uapprox = griddata( x, y, u, xex, yex, 'v4' ); 
err = sum( abs( uex - uapprox ).^2 )^(1/2);

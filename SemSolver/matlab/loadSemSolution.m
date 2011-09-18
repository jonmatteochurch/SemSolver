function [ x , y , u ] = loadSemSolution( path )

values = importdata( path );
x = values( :, 1 );
y = values( :, 2 );
u = values( :, 3 );
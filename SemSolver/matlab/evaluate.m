load 'poisson_ex';
[x12,y12,u12] = loadSemSolution('square_1_poisson_dir_hom_4_deg_2_lu.semsln');
[x13,y13,u13] = loadSemSolution('square_1_poisson_dir_hom_4_deg_3_lu.semsln');
[x14,y14,u14] = loadSemSolution('square_1_poisson_dir_hom_4_deg_4_lu.semsln');
[x18,y18,u18] = loadSemSolution('square_1_poisson_dir_hom_4_deg_8_lu.semsln');
[x22,y22,u22] = loadSemSolution('square_2_poisson_dir_hom_4_deg_2_lu.semsln');
[x23,y23,u23] = loadSemSolution('square_2_poisson_dir_hom_4_deg_3_lu.semsln');
[x24,y24,u24] = loadSemSolution('square_2_poisson_dir_hom_4_deg_4_lu.semsln');
[x28,y28,u28] = loadSemSolution('square_2_poisson_dir_hom_4_deg_8_lu.semsln');
[x42,y42,u42] = loadSemSolution('square_4_poisson_dir_hom_4_deg_2_lu.semsln');
[x43,y43,u43] = loadSemSolution('square_4_poisson_dir_hom_4_deg_3_lu.semsln');
[x44,y44,u44] = loadSemSolution('square_4_poisson_dir_hom_4_deg_4_lu.semsln');
[x48,y48,u48] = loadSemSolution('square_4_poisson_dir_hom_4_deg_8_lu.semsln');
[x122,y122,u122] = loadSemSolution('square_12_poisson_dir_hom_4_deg_2_lu.semsln');
[x123,y123,u123] = loadSemSolution('square_12_poisson_dir_hom_4_deg_3_lu.semsln');
[x124,y124,u124] = loadSemSolution('square_12_poisson_dir_hom_4_deg_4_lu.semsln');
[x128,y128,u128] = loadSemSolution('square_12_poisson_dir_hom_4_deg_8_lu.semsln');

N = [2, 3, 4, 8];
M = [1, 2, 4, 12];

err(1, 1) = normL2error(x12,y12,u12,xex,yex,uex);
err(1, 2) = normL2error(x13,y13,u13,xex,yex,uex);
err(1, 3) = normL2error(x14,y14,u14,xex,yex,uex);
err(1, 4) = normL2error(x18,y18,u18,xex,yex,uex);
err(2, 1) = normL2error(x22,y22,u22,xex,yex,uex);
err(2, 2) = normL2error(x23,y23,u23,xex,yex,uex);
err(2, 3) = normL2error(x24,y24,u24,xex,yex,uex);
err(2, 4) = normL2error(x28,y28,u28,xex,yex,uex);
err(3, 1) = normL2error(x42,y42,u42,xex,yex,uex);
err(3, 2) = normL2error(x43,y43,u43,xex,yex,uex);
err(3, 3) = normL2error(x44,y44,u44,xex,yex,uex);
err(3, 4) = normL2error(x48,y48,u48,xex,yex,uex);
err(4, 1) = normL2error(x122,y122,u122,xex,yex,uex);
err(4, 2) = normL2error(x123,y123,u123,xex,yex,uex);
err(4, 3) = normL2error(x124,y124,u124,xex,yex,uex);
err(4, 4) = normL2error(x128,y128,u128,xex,yex,uex);

figure(1)
loglog(M, err )

figure(2)
loglog(N, err')


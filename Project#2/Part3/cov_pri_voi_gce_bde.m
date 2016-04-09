%% cov for segmentation by CCP method
clear all
clc
% read the groundtruth and computed sementation
load man_groundtruth  % load rhinos_groundtruth
load Man_CCP          % load Rhinos_CCP
gt.Boundaries = [];
gt.Segmentation = im2uint8(man);  %im2uint8(rhinos)
groundTruth = {gt};
seg = double(im2uint8(color_segment));
% compute the cov
[cov]=compare_covering(seg,groundTruth);
%% cov for segmentation by MS method
clear all
clc
load man_groundtruth % load rhinos_groundtruth
load Man_MS          % load Rhinos_MS
gt.Boundaries = [];
gt.Segmentation = im2uint8(man); % rhinos
groundTruth = {gt};
seg = double(im2uint8(y));
% compute the cov
[cov]=compare_covering(seg,groundTruth);
%% PRI,VOI and GCE for segmentation by CCP method 
clear all
clc
load Man_MS; % load Rhinos_MS
load man_groundtruth; % rhinos_groundtruth
load Man_CCP; % Man_CCP
man_ccp = im2uint8(color_segment);
man_ms = uint8(y);
% compute the POI, VOI and GCE
[ri_ccp,gce_ccp,vi_ccp]=compare_segmentations(man_ccp,man);
[ri_ms,gce_ms,vi_ms]=compare_segmentations(man_ms,man);
%% BDE for segmentation by CCP
clear all
clc
% read the computed segmentation and groundtruth
load Man_GT % load Rhinos_GT
load Man_CCP % load Man_CCP
gt = groundTruth{1}.Segmentation;
sum = 0;
for i=1:1:3
   seg = color_segment(:,:,i);
   averageError= compare_image_boundary_error((seg), double(gt)); 
   sum = averageError+sum;
end
% compute the BDE
bde = sum/3;
%% BDE for segmentation by MS
clear all
clc
load Man_GT % load Rhinos_GT
load Man_MS % load Rhinos_MS
gt = groundTruth{1}.Segmentation;
sum = 0;
for i=1:1:3
   seg = y(:,:,i);
   averageError= compare_image_boundary_error(seg, double(gt)); 
   sum = averageError+sum;
end
bde = sum/3;
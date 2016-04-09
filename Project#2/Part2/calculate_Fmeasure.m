%% calculate the precision and recall for SE detector
clear all
clc
% read the groundtruth
load cougar
precision_opt = [];
recall_opt = [];
for k=1:1:5
 G = groundTruth(k); % select the groundtruth
 % calculate the precision and recall for each threshold
 [thrs,cntR,sumR,cntP,sumP,V] = edgesEvalImg(E,G); % E is the edge map
 precision = cntP./sumP;
 recall = cntR./sumR;
 % calculate the F-measure
 F = 2.*(precision.*recall)./(precision+recall);
 max = F(1);
 threshold = 1;
 % find the max F-measure and corresponding prescision and recall
 for i=1:1:99
     if max<F(i)
         max = F(i);
         threshold = i;
     end
 end
 precision_opt(k) = precision(threshold);
 recall_opt(k) = recall(threshold);  
end
%% calculate the precision and recall for sobel, nms and canny detector
E = imread('farm_edge_10');
E = single(E./255);
precision = zeros(5,1);
recall = zeros(5,1);
%F = zeros(5,1);
for i=1:1:5
    G = groundTruth(i);
    [thrs,cntR,sumR,cntP,sumP,V] = edgesEvalImg(1-E,G,'thrs',1);
    precision(i) = cntP./sumP;
    recall(i) = cntR./sumR;
    %F(i) = 2.*(precision.*recall)./(precision+recall);
end
%% classifier using minimum distance, feature reduction by PCA
clear all
clc
% read the sample data
raw = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification_b/texture_classification_b/part_b_new.xlsx');
sample = (reshape(raw,[25,192]))';
sum = zeros(4,2);
for rtime=1:1:20 % run the classifier 20 times to get average error rate
% randomly select trainning samples
grass = sample(1:48,:);leather = sample(49:96,:);sand = sample(97:144,:);straw = sample(145:192,:);
rnd1 = randperm(48);rnd2 = randperm(48);rnd3 = randperm(48);rnd4 = randperm(48);
grass = grass(rnd1,:);leather = leather(rnd2,:);sand = sand(rnd3,:);straw = straw(rnd4,:);
train_grass = grass(1:36,:);train_leather = leather(1:36,:);train_sand = sand(1:36,:);train_straw = straw(1:36,:);
% using the trainning samples for normalization
K = [train_grass;train_leather;train_sand;train_straw];
max = zeros(1,25);
min = 99999999*ones(1,25);
for j=1:1:25
    for i=1:1:144
        if min(j)>K(i,j)
            min(j) = K(i,j);
        end
        if max(j)<K(i,j)
            max(j) = K(i,j);
        end
    end
end
for j=1:1:25
    for i=1:1:192
        sample(i,j) = (sample(i,j)-min(j))/(max(j)-min(j));
    end
end  
grass = sample(1:48,:);leather = sample(49:96,:);sand = sample(97:144,:);straw = sample(145:192,:);
grass = grass(rnd1,:);leather = leather(rnd2,:);sand = sand(rnd3,:);straw = straw(rnd4,:);
train_grass = grass(1:36,:);train_leather = leather(1:36,:);train_sand = sand(1:36,:);train_straw = straw(1:36,:);
% applying PCA for feature dimension reduction
[coeff,score,latent] = pca1([train_grass;train_leather;train_sand;train_straw]);
co = coeff(:,1:3); % set the desired reduced dimension
% define the trainning and test samples
train_grass = grass(1:36,:)*co;train_leather = leather(1:36,:)*co;train_sand = sand(1:36,:)*co;train_straw = straw(1:36,:)*co;
test_grass = grass(37:48,:)*co;test_leather = leather(37:48,:)*co;test_sand = sand(37:48,:)*co;test_straw = straw(37:48,:)*co;
test_1 = [test_grass;test_leather(1:4,:);test_sand(1:4,:);test_straw(1:4,:)];
test_2 = [test_leather;test_grass(1:4,:);test_sand(1:4,:);test_straw(1:4,:)];
test_3 = [test_sand;test_grass(1:4,:);test_leather(1:4,:);test_straw(1:4,:)];
test_4 = [test_straw;test_grass(1:4,:);test_leather(1:4,:);test_sand(1:4,:)];
% define the training and test labels
test_label1 = [zeros(1,36),ones(1,36)];
test_label2 = [zeros(1,12),ones(1,12)];
label1 = zeros(1,72);
label2 = zeros(1,24);
% Grass VS Non-grass classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_leather(rnd1,:);neg_leather = tmp1(1:12,:);
tmp2 = train_sand(rnd2,:);neg_sand = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_leather;neg_sand;neg_straw];
train_two = [train_grass;neg_train];
% traing the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_grass);
cov_neg = cov(neg_train);
mean_pos = mean(train_grass);
mean_neg = mean(neg_train);
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e11 = mean(test_label1~=label1);
sum(1,1) = sum(1,1)+e11;
for i=1:1:24 
    d_pos = (test_1(i,:)-mean_pos) * inv(cov_pos) * (test_1(i,:)-mean_pos)';
    d_neg = (test_1(i,:)-mean_neg) * inv(cov_neg) * (test_1(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e12 = mean(test_label2~=label2);
sum(1,2) = sum(1,2)+e12;
% Leather VS Non-leather classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_sand(rnd2,:);neg_sand = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_grass;neg_sand;neg_straw];
train_two = [train_leather;neg_train];
% traing the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_leather);
cov_neg = cov(neg_train);
mean_pos = mean(train_leather);
mean_neg = mean(neg_train);
% calculate the training and test error rate
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e21 = mean(test_label1~=label1);
sum(2,1) = sum(2,1)+e21;
for i=1:1:24 
    d_pos = (test_2(i,:)-mean_pos) * inv(cov_pos) * (test_2(i,:)-mean_pos)';
    d_neg = (test_2(i,:)-mean_neg) * inv(cov_neg) * (test_2(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e22 = mean(test_label2~=label2);
sum(2,2) = sum(2,2)+e22;
% Sand VS Non-sand classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_leather(rnd2,:);neg_leather = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_grass;neg_leather;neg_straw];
train_two = [train_sand;neg_train];
% traing the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_sand);
cov_neg = cov(neg_train);
mean_pos = mean(train_sand);
mean_neg = mean(neg_train);
% calculate the training and test error rate
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e31 = mean(test_label1~=label1);
sum(3,1) = sum(3,1)+e31;
for i=1:1:24 
    d_pos = (test_3(i,:)-mean_pos) * inv(cov_pos) * (test_3(i,:)-mean_pos)';
    d_neg = (test_3(i,:)-mean_neg) * inv(cov_neg) * (test_3(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e32 = mean(test_label2~=label2);
sum(3,2) = sum(3,2)+e32;
% Straw VS Non-straw classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_leather(rnd2,:);neg_leather = tmp2(1:12,:);
tmp3 = train_sand(rnd3,:);neg_sand = tmp3(1:12,:);
neg_train = [neg_grass;neg_leather;neg_sand];
train_two = [train_straw;neg_train];
% traing the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_straw);
cov_neg = cov(neg_train);
mean_pos = mean(train_straw);
mean_neg = mean(neg_train);
% calculate the training and test error rate
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e41 = mean(test_label1~=label1);
sum(4,1) = sum(4,1)+e41;
for i=1:1:24
    d_pos = (test_4(i,:)-mean_pos) * inv(cov_pos) * (test_4(i,:)-mean_pos)';
    d_neg = (test_4(i,:)-mean_neg) * inv(cov_neg) * (test_4(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e42 = mean(test_label2~=label2);
sum(4,2) = sum(4,2)+e42;
end
aver_error_rate = sum./20;
%% plot the samples distribution in feature space for PCA method
scatter3(train_two(1:36,1),train_two(1:36,2),train_two(1:36,3),'o','y');
hold on
scatter3(mean_pos(1),mean_pos(2),mean_pos(3),'x','r');
hold on
scatter3(train_two(37:72,1),train_two(37:72,2),train_two(37:72,3),'o','g');
hold on
scatter3(mean_neg(1),mean_neg(2),mean_neg(3),'x','r');
%% classifier using minimum distance, feature reduction by PCA
clear all
clc
% read the sample data
raw = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification_b/texture_classification_b/part_b_new.xlsx');
sample = (reshape(raw,[25,192]))';
sum = zeros(4,2);
for rtime=1:1:20  % run the classifier 20 times to get average error rate
% randomly select trainning samples and test samples
grass = sample(1:48,:);leather = sample(49:96,:);sand = sample(97:144,:);straw = sample(145:192,:);
rnd1 = randperm(48);rnd2 = randperm(48);rnd3 = randperm(48);rnd4 = randperm(48);
grass = grass(rnd1,:);leather = leather(rnd2,:);sand = sand(rnd3,:);straw = straw(rnd4,:);
train_grass = grass(1:36,:);train_leather = leather(1:36,:);train_sand = sand(1:36,:);train_straw = straw(1:36,:);
% using the trainning samples for normalization
K = [train_grass;train_leather;train_sand;train_straw];
max = zeros(1,25);
min = 99999999*ones(1,25);
for j=1:1:25
    for i=1:1:144
        if min(j)>K(i,j)
            min(j) = K(i,j);
        end
        if max(j)<K(i,j)
            max(j) = K(i,j);
        end
    end
end
for j=1:1:25
    for i=1:1:192
        sample(i,j) = (sample(i,j)-min(j))/(max(j)-min(j));
    end
end
% define the trainning and test samples
grass = sample(1:48,:);leather = sample(49:96,:);sand = sample(97:144,:);straw = sample(145:192,:);
grass = grass(rnd1,:);leather = leather(rnd2,:);sand = sand(rnd3,:);straw = straw(rnd4,:);
train_grass = grass(1:36,:);train_leather = leather(1:36,:);train_sand = sand(1:36,:);train_straw = straw(1:36,:);
test_grass = grass(37:48,:);test_leather = leather(37:48,:);test_sand = sand(37:48,:);test_straw = straw(37:48,:);
test_1 = [test_grass;test_leather(1:4,:);test_sand(1:4,:);test_straw(1:4,:)];
test_2 = [test_leather;test_grass(1:4,:);test_sand(1:4,:);test_straw(1:4,:)];
test_3 = [test_sand;test_grass(1:4,:);test_leather(1:4,:);test_straw(1:4,:)];
test_4 = [test_straw;test_grass(1:4,:);test_leather(1:4,:);test_sand(1:4,:)];
% the training and test labels
test_label1 = [zeros(1,36),ones(1,36)];
test_label2 = [zeros(1,12),ones(1,12)];
label1 = zeros(1,72);
label2 = zeros(1,24);
% Grass VS Non-grass classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_leather(rnd1,:);neg_leather = tmp1(1:12,:);
tmp2 = train_sand(rnd2,:);neg_sand = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_leather;neg_sand;neg_straw];
train_two = [train_grass;neg_train];
% applying LDA for feature dimension reduction
[w,centers] = LDA(train_two,test_label1);
train_two = train_two * w;
test_1 = test_1 * w;
% training the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_two(1:36,:));
cov_neg = cov(train_two(37:72,:));
mean_pos = mean(train_two(1:36,:));
mean_neg = mean(train_two(37:72,:));
% calculate the training and test error rate
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e11 = mean(test_label1~=label1);
sum(1,1) = sum(1,1)+e11;
for i=1:1:24 
    d_pos = (test_1(i,:)-mean_pos) * inv(cov_pos) * (test_1(i,:)-mean_pos)';
    d_neg = (test_1(i,:)-mean_neg) * inv(cov_neg) * (test_1(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e12 = mean(test_label2~=label2);
sum(1,2) = sum(1,2)+e12;
% Leather VS Non-leather classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_sand(rnd2,:);neg_sand = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_grass;neg_sand;neg_straw];
train_two = [train_leather;neg_train];
% applying LDA for feature dimension reduction
[w,centers] = LDA(train_two,test_label1);
train_two = train_two * w;
test_2 = test_2 * w;
% training the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_two(1:36,:));
cov_neg = cov(train_two(37:72,:));
mean_pos = mean(train_two(1:36,:));
mean_neg = mean(train_two(37:72,:));
% calculate the training and test error rate
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e21 = mean(test_label1~=label1);
sum(2,1) = sum(2,1)+e21;
for i=1:1:24 
    d_pos = (test_2(i,:)-mean_pos) * inv(cov_pos) * (test_2(i,:)-mean_pos)';
    d_neg = (test_2(i,:)-mean_neg) * inv(cov_neg) * (test_2(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e22 = mean(test_label2~=label2);
sum(2,2) = sum(2,2)+e22;
% Sand VS Non-sand classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_leather(rnd2,:);neg_leather = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_grass;neg_leather;neg_straw];
train_two = [train_sand;neg_train];
% applying LDA for feature dimension reduction
[w,centers] = LDA(train_two,test_label1);
train_two = train_two * w;
test_3 = test_3 * w;
% training the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_two(1:36,:));
cov_neg = cov(train_two(37:72,:));
mean_pos = mean(train_two(1:36,:));
mean_neg = mean(train_two(37:72,:));
% calculate the training and test error rate
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e31 = mean(test_label1~=label1);
sum(3,1) = sum(3,1)+e31;
for i=1:1:24 
    d_pos = (test_3(i,:)-mean_pos) * inv(cov_pos) * (test_3(i,:)-mean_pos)';
    d_neg = (test_3(i,:)-mean_neg) * inv(cov_neg) * (test_3(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e32 = mean(test_label2~=label2);
sum(3,2) = sum(3,2)+e32;
% Straw VS Non-straw classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_leather(rnd2,:);neg_leather = tmp2(1:12,:);
tmp3 = train_sand(rnd3,:);neg_sand = tmp3(1:12,:);
neg_train = [neg_grass;neg_leather;neg_sand];
train_two = [train_straw;neg_train];
% applying LDA for feature dimension reduction
[w,centers] = LDA(train_two,test_label1);
train_two = train_two * w;
test_4 = test_4 * w;
% training the classifier using traing data by minimum Mahalanobis distance
cov_pos = cov(train_two(1:36,:));
cov_neg = cov(train_two(37:72,:));
mean_pos = mean(train_two(1:36,:));
mean_neg = mean(train_two(37:72,:));
% calculate the training and test error rate
for i=1:1:72 
    d_pos = (train_two(i,:)-mean_pos) * inv(cov_pos) * (train_two(i,:)-mean_pos)';
    d_neg = (train_two(i,:)-mean_neg) * inv(cov_neg) * (train_two(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
e41 = mean(test_label1~=label1);
sum(4,1) = sum(4,1)+e41;
for i=1:1:24
    d_pos = (test_4(i,:)-mean_pos) * inv(cov_pos) * (test_4(i,:)-mean_pos)';
    d_neg = (test_4(i,:)-mean_neg) * inv(cov_neg) * (test_4(i,:)-mean_neg)';
    if(d_pos<d_neg)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
e42 = mean(test_label2~=label2);
sum(4,2) = sum(4,2)+e42;
end
aver_error_rate = sum./20;
%% plot the samples distribution in feature space for PCA method
scatter(train_two(1:36,:),zeros(36,1),'o','r')
hold on
scatter(train_two(37:72,:),zeros(36,1),'o','b')
hold on
scatter(test_4,zeros(24,1),'.','g')
%% classifier using SVM
clear all
clc
% read the sample data
raw = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification_b/texture_classification_b/part_b_new.xlsx');
sample = (reshape(raw,[25,192]))';
sum = zeros(4,2);
for rtime=1:1:20 % run the classifier 20 times to get average error rate
% randomly select trainning samples and test samples
grass = sample(1:48,:);leather = sample(49:96,:);sand = sample(97:144,:);straw = sample(145:192,:);
rnd1 = randperm(48);rnd2 = randperm(48);rnd3 = randperm(48);rnd4 = randperm(48);
grass = grass(rnd1,:);leather = leather(rnd2,:);sand = sand(rnd3,:);straw = straw(rnd4,:);
train_grass = grass(1:36,:);train_leather = leather(1:36,:);train_sand = sand(1:36,:);train_straw = straw(1:36,:);
% using the trainning samples for normalization
K = [train_grass;train_leather;train_sand;train_straw];
max = zeros(1,25);
min = 99999999*ones(1,25);
for j=1:1:25
    for i=1:1:144
        if min(j)>K(i,j)
            min(j) = K(i,j);
        end
        if max(j)<K(i,j)
            max(j) = K(i,j);
        end
    end
end
for j=1:1:25
    for i=1:1:192
        sample(i,j) = (sample(i,j)-min(j))/(max(j)-min(j));
    end
end  
% define the trainning and test samples
grass = sample(1:48,:);leather = sample(49:96,:);sand = sample(97:144,:);straw = sample(145:192,:);
grass = grass(rnd1,:);leather = leather(rnd2,:);sand = sand(rnd3,:);straw = straw(rnd4,:);
train_grass = grass(1:36,:);train_leather = leather(1:36,:);train_sand = sand(1:36,:);train_straw = straw(1:36,:);
test_grass = grass(37:48,:);test_leather = leather(37:48,:);test_sand = sand(37:48,:);test_straw = straw(37:48,:);
test_1 = [test_grass;test_leather(1:4,:);test_sand(1:4,:);test_straw(1:4,:)];
test_2 = [test_leather;test_grass(1:4,:);test_sand(1:4,:);test_straw(1:4,:)];
test_3 = [test_sand;test_grass(1:4,:);test_leather(1:4,:);test_straw(1:4,:)];
test_4 = [test_straw;test_grass(1:4,:);test_leather(1:4,:);test_sand(1:4,:)];
% the training and test labels
label1 = [ones(1,36),-1*ones(1,36)]';
label2 = [ones(1,12),-1*ones(1,12)]';
% set SVM type "C-svc" and kernel type to "linear"
params = ['-t 0 -s 0'];
% Grass VS Non-grass classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_leather(rnd1,:);neg_leather = tmp1(1:12,:);
tmp2 = train_sand(rnd2,:);neg_sand = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_leather;neg_sand;neg_straw];
train_two = [train_grass;neg_train];
% training model using training data
model = svmtrain(label1,train_two,params);
% calculate the training and test error rate
predicted_label= svmpredict(label1,train_two,model);
e11 = mean(predicted_label~=label1);
sum(1,1) = sum(1,1)+e11;
predicted_label= svmpredict(label2,test_1,model);
e12 = mean(predicted_label~=label2);
sum(1,2) = sum(1,2)+e12;
%Leather VS Non-leather classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_sand(rnd2,:);neg_sand = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_grass;neg_sand;neg_straw];
train_two = [train_leather;neg_train];
% training model using training data
model = svmtrain(label1,train_two,params);
% calculate the training and test error rate
predicted_label= svmpredict(label1,train_two,model);
e21 = mean(predicted_label~=label1);
sum(2,1) = sum(2,1)+e21;
predicted_label= svmpredict(label2,test_2,model);
e22 = mean(predicted_label~=label2);
sum(2,2) = sum(2,2)+e22;
% Sand VS Non-sand classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_leather(rnd2,:);neg_leather = tmp2(1:12,:);
tmp3 = train_straw(rnd3,:);neg_straw = tmp3(1:12,:);
neg_train = [neg_grass;neg_leather;neg_straw];
train_two = [train_sand;neg_train];
% training model using training data
model = svmtrain(label1,train_two,params);
predicted_label= svmpredict(label1,train_two,model);
e31 = mean(predicted_label~=label1);
sum(3,1) = sum(3,1)+e31;
predicted_label= svmpredict(label2,test_3,model);
e32 = mean(predicted_label~=label2);
sum(3,2) = sum(3,2)+e32;
% Straw VS Non-straw classifier
rnd1 = randperm(36);rnd2 = randperm(36);rnd3 = randperm(36);
tmp1 = train_grass(rnd1,:);neg_grass = tmp1(1:12,:);
tmp2 = train_leather(rnd2,:);neg_leather = tmp2(1:12,:);
tmp3 = train_sand(rnd3,:);neg_sand = tmp3(1:12,:);
neg_train = [neg_grass;neg_leather;neg_sand];
train_two = [train_straw;neg_train];
% training model using training data
model = svmtrain(label1,train_two,params);
% calculate the training and test error rate
predicted_label= svmpredict(label1,train_two,model);
e41 = mean(predicted_label~=label1);
sum(4,1) = sum(4,1)+e41;
predicted_label= svmpredict(label2,test_4,model);
e42 = mean(predicted_label~=label2);
sum(4,2) = sum(4,2)+e42;
end
aver_error_rate = sum./20;
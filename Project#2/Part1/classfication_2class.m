%% classifier using minimum distance for 25 featues 
clear all
clc
% read the sample data
raw = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification/texture_classification/part_a_1.xlsx');
% read the test labels
label_test = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification/texture_classification/test_label.xlsx');
% rearrange the samples, the row stands for sample, the column stands for
% features
sample = (reshape(raw,[25,96]))';
% feature normalization
max = zeros(1,25);
min = zeros(1,25);
for j=1:1:25
    for i=1:1:72
        if min(j)>sample(i,j)
            min(j) = sample(i,j);
        end
        if max(j)<sample(i,j)
            max(j) = sample(i,j);
        end
    end
end
for j=1:1:25
    for i=1:1:96
        sample(i,j) = (sample(i,j)-min(j))/(max(j)-min(j));
    end
end
% define training data and test data
train = sample(1:72,:);
train_grass = sample(1:36,:);
train_straw = sample(37:72,:);
test = sample(73:96,:);
% traing the classifier using traing data by minimum Mahalanobis distance 
cov_grass = cov(train_grass);
cov_straw = cov(train_straw);
mean_grass = mean(train_grass);
mean_straw = mean(train_straw);
label_train = [zeros(1,36),ones(1,36)];
label1 = [];
label2 = [];
% calculate the training and test error rate
for i=1:1:72 
    d_grass = (train(i,:)-mean_grass) * inv(cov_grass) * (train(i,:)-mean_grass)';
    d_straw = (train(i,:)-mean_straw) * inv(cov_straw) * (train(i,:)-mean_straw)';
    if(d_grass<d_straw)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
mean(label_train~=label1)
for i=1:1:24 
    d_grass = (test(i,:)-mean_grass) * inv(cov_grass) * (test(i,:)-mean_grass)';
    d_straw = (test(i,:)-mean_straw) * inv(cov_straw) * (test(i,:)-mean_straw)';
    if(d_grass<d_straw)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
mean(label_test~=label2)
%% classifier using minimum distance, feature reduction by PCA
clear all
clc
% read the sample data
raw = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification/texture_classification/part_a_1.xlsx');
% read the test labels
test_label = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification/texture_classification/test_label.xlsx');
% rearrange the samples, the row stands for sample, the column stands for
% features
sample = (reshape(raw,[25,96]))';
% feature normalization
max = zeros(1,25);
min = zeros(1,25);
for j=1:1:25
    for i=1:1:72
        if min(j)>sample(i,j)
            min(j) = sample(i,j);
        end
        if max(j)<sample(i,j)
            max(j) = sample(i,j);
        end
    end
end
for j=1:1:25
    for i=1:1:96
        sample(i,j) = (sample(i,j)-min(j))/(max(j)-min(j));
    end
end
% define training data and test data
train_label = [zeros(1,36),ones(1,36)];
train = sample(1:72,:);
% applying PCA for feature dimension reduction
[coeff,score,latent] = pca1(train);
sample_new = sample*coeff(:,1); % set the desired reduced dimension
train_grass = sample_new(1:36,:);
train_straw = sample_new(37:72,:);
train_two = [train_grass;train_straw];
test = sample_new(73:96,:);
% traing the classifier using traing data by minimum Mahalanobis distance
cov_grass = cov(train_grass);
cov_straw = cov(train_straw);
mean_grass = mean(train_grass);
mean_straw = mean(train_straw);
label1 = [];
label2 = [];
% calculate the training and test error rate
for i=1:1:72 
    d_grass = (train_two(i,:)-mean_grass) * inv(cov_grass) * (train_two(i,:)-mean_grass)';
    d_straw = (train_two(i,:)-mean_straw) * inv(cov_straw) * (train_two(i,:)-mean_straw)';
    if(d_grass<d_straw)
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
mean(train_label~=label1)
for i=1:1:24 
    d_grass = (test(i,:)-mean_grass) * inv(cov_grass) * (test(i,:)-mean_grass)';
    d_straw = (test(i,:)-mean_straw) * inv(cov_straw) * (test(i,:)-mean_straw)';
    if(d_grass<d_straw)
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
mean(test_label~=label2)
%% plot the samples distribution in feature space for PCA method
scatter(train_grass,zeros(36,1),'o','r')
hold on
scatter(train_straw,zeros(36,1),'o','b')
hold on
scatter(test,zeros(24,1),'.','g')
%% classifier using minimum distance, feature reduction by LDA
clear all
clc
% read the sample data
raw = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification/texture_classification/part_a_1.xlsx');
% read the test labels
label_test = xlsread('/Users/YiZheng/Desktop/Codes/texture_classification/texture_classification/test_label.xlsx');
% rearrange the samples, the row stands for sample, the column stands for
% features
sample = (reshape(raw,[25,96]))';
% feature normalization
max = zeros(1,25);
min = zeros(1,25);
for j=1:1:25
    for i=1:1:72
        if min(j)>sample(i,j)
            min(j) = sample(i,j);
        end
        if max(j)<sample(i,j)
            max(j) = sample(i,j);
        end
    end
end
for j=1:1:25
    for i=1:1:96
        sample(i,j) = (sample(i,j)-min(j))/(max(j)-min(j));
    end
end
% define training data and test data
train = sample(1:72,:);
label_train = [zeros(1,36) ones(1,36)];
test = sample(73:96,:);
% traing the classifier using traing data by minimum Mahalanobis distance
[w,centers] = LDA(train,label_train);
train_project = train * w;
test_project = test * w;
cov1 = cov(train_project(1:36,:));
cov2 = cov(train_project(37:72,:));
label1 = [];
label2 = [];
% calculate the training and test error rate
for i=1:1:72
    if(((train_project(i)-centers(1))^2/cov1)<((train_project(i)-centers(2))^2/cov2))
        label1(i) = 0;
    else
        label1(i) = 1;
    end
end
mean(label_train~=label1)
for i=1:1:24
    if(((test_project(i)-centers(1))^2/cov1)<((test_project(i)-centers(2))^2/cov2))
        label2(i) = 0;
    else
        label2(i) = 1;
    end
end
mean(label_test~=label2)
%% plot the samples distribution in feature space for LDA method
scatter(train_project(1:36,:),zeros(36,1),'o','r')
hold on
scatter(train_project(37:72,:),zeros(36,1),'o','b')
hold on
scatter(test_project,zeros(24,1),'.','g')
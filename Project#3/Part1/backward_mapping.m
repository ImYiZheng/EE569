%% read input images
% read the images on face #1, #2 and #3
baby = im2double(imread('baby.jpg'));
baby_cat = im2double(imread('baby_cat.jpg'));
baby_cat = flipud(baby_cat);
baby_dog = im2double(imread('baby_dog.jpg'));
baby_dog = fliplr(flipud(baby_dog));
%% set the known parameters 
a = 0;
b = 0;
p = 0;
q = 0;
% density obtained from forward mapping
density = 280;
cx = 100/density;
cy = 100/density;
% output image
out = zeros(200,200,3);
% normal vector for three faces
n1 = [0,0,1];
n2 = [1,0,0];
n3 = [0,1,0];
d = 0.01;
% x,y,z coordinate in world coordinate
X = zeros(200,200);
Y = zeros(200,200);
Z = zeros(200,200);
% find address for the output image in the Cartesian coordinate
out_x = zeros(200,200);
out_y = zeros(200,200);
for i=1:1:200
    for j=1:1:200
      out_x(i,j) = (j-1)/density;  
      out_y(i,j) = (199-i)/density;
    end
end
% extrinsic matrix
Rt = [-1/sqrt(2),1/sqrt(2),0;
      1/sqrt(6),1/sqrt(6),-2/sqrt(6);
     -1/sqrt(3),-1/sqrt(3),-1/sqrt(3);
      ];
% intrinsic matrix
K= [sqrt(3),0,cx;
    0,sqrt(3),cy;
    0,0,1];
KR = K*Rt;
C = [5;5;5];
%% reverse mapping
% for plane #1 Z is fixed
for i=1:1:200
    for j=1:1:200
    % find the reverse mapping coordinate 
    P = ((-d-n1*C)/(n1*(inv(KR)*[out_x(i,j);out_y(i,j);1])))*inv(KR)*[out_x(i,j);out_y(i,j);1]+C;
    D3 = (P+1)*100+100;
    X(i,j) = D3(1);
    Y(i,j) = D3(2);
    Z(i,j) = D3(3);
    end
end
for i=1:1:200
    for j=1:1:200
        % bilinear interpolation
        if(X(i,j)>=1&&X(i,j)<=199&&Y(i,j)>=1&&Y(i,j)<=199)
            p = floor(X(i,j));
            q = floor(Y(i,j));
            a = X(i,j)-p;
            b = Y(i,j)-q;
            out(i,j,:) = (1-a)*(1-b)*baby(p,q,:)+b*(1-a)*baby(p,q+1,:)+a*(1-b)*baby(p+1,q,:)+a*b*baby(p+1,q+1,:);
        end
    end
end
% for plane #2 X is fixed
for i=1:1:200
    for j=1:1:200
    % find the reverse mapping coordinate
    P = ((-d-n2*C)/(n2*(inv(KR)*[out_x(i,j);out_y(i,j);1])))*inv(KR)*[out_x(i,j);out_y(i,j);1]+C;
    D3 = (P+1)*100+100;
    X(i,j) = D3(1);
    Y(i,j) = D3(2);
    Z(i,j) = D3(3);
    end
end
for i=1:1:200
    for j=1:1:200
        % bilinear interpolation
        if(Z(i,j)>=1&&Z(i,j)<=199&&Y(i,j)>=1&&Y(i,j)<=199)
            p = floor(Z(i,j));
            q = floor(Y(i,j));
            a = Z(i,j)-p;
            b = Y(i,j)-q;
            out(i,j,:) = (1-a)*(1-b)*baby_cat(p,q,:)+b*(1-a)*baby_cat(p,q+1,:)+a*(1-b)*baby_cat(p+1,q,:)+a*b*baby_cat(p+1,q+1,:);
        end
    end
end
% for plane #3 Y is fixed
for i=1:1:200
    for j=1:1:200
    % find the reverse mapping coordinate
    P = ((-d-n3*C)/(n3*(inv(KR)*[out_x(i,j);out_y(i,j);1])))*inv(KR)*[out_x(i,j);out_y(i,j);1]+C;
    D3 = (P+1)*100+100;
    X(i,j) = D3(1);
    Y(i,j) = D3(2);
    Z(i,j) = D3(3);
    end
end
for i=1:1:200
    for j=1:1:200
        % bilinear interpolation
        if(X(i,j)>=1&&X(i,j)<=199&&Z(i,j)>=1&&Z(i,j)<=199)
            p = floor(Z(i,j));
            q = floor(X(i,j));
            a = Z(i,j)-p;
            b = X(i,j)-q;
            out(i,j,:) = (1-a)*(1-b)*baby_dog(p,q,:)+b*(1-a)*baby_dog(p,q+1,:)+a*(1-b)*baby_dog(p+1,q,:)+a*b*baby_dog(p+1,q+1,:);
        end
    end
end
% show the output image
imshow(im2uint8(out));
clc;
close all;
data=load('/Users/mansi/Documents/file.txt','-ascii');
coeff=pca(data,'NumComponents',5);
finaldata=data*coeff;
dlmwrite('/Users/mansi/Documents/file_matlab.txt',finaldata,'\t');

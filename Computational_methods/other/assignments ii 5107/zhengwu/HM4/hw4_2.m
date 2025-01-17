%HM4 Second Problem
clear; close all;

load('hw4_2_data.mat');

N = 100;
X = X4;
XX = X;

H = 4;
J = -2;

%define the neighbor
for i = 1:N
    for j = 1:N
        temp = [i-1,j; i+1,j; i,j-1; i,j+1];
        ngh{i,j} = temp(min(temp, [], 2) >= 1 & max(temp, [], 2) <= N, :);
    end
end

%K sweeps
K = 5;
for k = 2:K
    for i = 1:N
        for j=1:N
            mu = 0;
            for r=1:length(ngh{i,j})
                mu = mu + X(ngh{i,j}(r,1),ngh{i,j}(r,2));
            end
            mu=mu/length(ngh{i,j});
            U =mu+sqrt(0.1)*randn(1);
            X(i,j) = U;
           % X(i,j) = random('normal',mu,0.1);
        end
    end
    XX(:,:,k) = X;
end


for k=1:K
    subplot(2,3,k);
    imagesc(XX(:,:,k));
    colormap(gray);
    title(sprintf('%d-th run', k));
   % pause; clf;
end
                
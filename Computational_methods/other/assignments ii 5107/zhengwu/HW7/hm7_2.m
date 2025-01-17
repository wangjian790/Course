%answer for HW7
%problem 2
clear all;
close all;

namta = 0.1;

%Generate 50 samples from exponential distribution 
e_mu = 1/namta;

e_s = exprnd(e_mu, 20,50);

for i=1:50
t(:,i) = cumsum(e_s(:,i));

temp = t(:,i);
P_t{i} = temp(temp<100);
hold on, plot(P_t{i}',i,'*')
end;

figure(2);
PP = [];
for i = 1:50
    PP = [PP P_t{i}'];
end

hist(PP,2.5:5:97.5);

figure (3);
for i=1:50
    r(i) = sum(P_t{i}>10 & P_t{i}<60);
end;

hist(r, 0:50);
% hold on, plot(P_t{i},i,'*')
% axis([0 100 0 50])
% end;

%do ks test
mu = mean(r);
x = 0:50;
est_cdf = poisscdf(x, mu);
[h,p] = kstest(r, [x' est_cdf']);

function[A, b, x] = forSub(A, b)
%Forward Substitution; solves for Ax = b when A is lower triangular
	n = length(b);
	for f = 1:n
		x(f) = 0;
	end
	x(1) = b(1)/A(1, 1);
	for i = 2:n
		soln = b(i);
		for j = 1:(i - 1)
			soln = soln - A(i, j)*x(j);
		end
		x(i) = soln/A(i, i);
	end
end	

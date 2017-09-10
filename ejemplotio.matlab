function [flujo_max, cf, residual, corte] =ff_flujo_max_jm4(origen,destino,capacidad,numero_nodos)

//flujo_actual es una matriz donde se guardaran los todos los paths, todas las iteracciones
flujo_actual=zeros(numero_nodos,numero_nodos); una matriz llena de zeros MATRIZ QUE CONTENDRÁ LOS CAMINOS DE UN SUMIDERO

flujo_max=0; GUARDA EL FLUJO MÁXIMO DE ESTE destino
pathaumentado = ff_pathaumentado(origen,destino,flujo_actual,capacidad,numero_nodos); BUSCA UN PATHAUMETADO CON ESTA

jp=0;el numero del path aumentado (es el primer path(contador pa comparar paths)) VARIABLE AGREGADA PARA COMPROBAR CUANDO ES EL PRIMER PATHAUMENTADO

// desde aqui lo q hace ford fulkerson
while ~isempty(pathaumentado)
	% Si existe un camino aumentado, actualice el flujo_actual
	incremento = inf;
	for i=1:length(pathaumentado)-1
		incremento=min(incremento, capacidad(pathaumentado(i),pathaumentado(i+1))-flujo_actual(pathaumentado(i),pathaumentado(i+1)));
	end
% Incremento en el flujo actual ACTUALIZA EL FLUJO ACTUAL CON EL PATHAUMENTADO ENCONTRADO
	for i=1:length(pathaumentado)-1
		flujo_actual(pathaumentado(i),pathaumentado(i+1))=flujo_actual(pathaumentado(i),pathaumentado(i+1))+incremento;
	end
	flujo_max=flujo_max+incremento;
	//hasta aqui es lo q hace ford fulkerson


	//desde aqui lo q hay q cambiar
	jp=jp+1; AUMENTA EL jp PARA TODOS LOS PATHAUMENTADOS, CUANDO SEA 1, CALCULAR OTRO pathaumentado
	if jp==1
		pathaumentado = ff_pathaumentado(origen,destino,flujo_actual,capacidad,numero_nodos);% try to find new augment path
	end

	//desde aqui compara los path aumentados
	if jp&gt;=2 CUANDO jp ES MAYOR QUE 2, EMPIEZA A PROBAR LOS SIGUIENTES PATHAUMENTADOS QUE CALCULE
		cf=flujo_actual; SACA UNA COPIA DEL flujo_actual

		ii=1; *** ESTE BLOQUE 1 LO EXPLICO ABAJO (ii es un contador desde el origen al destino)
		swe=false; (switch que dice que encontro o no encontro un nodo con dos links)

		while (ii~=destino) &amp;&amp; (~swe)
			tf=cf(:,ii); // tf vector columna literal, se va sobre escribiendo
			posc=find(tf==1); buscando las posiciones q tengan uno, en posc (vector) se guardan la posiciones donde se econtraron unos
			if length(posc)==2 si hay mas de un uno es un nodo q se repite en dos caminos
				swe=true; y se acaba bloque 1 
			else
				ii=ii+1;
			end
		end *** FIN DEL BLOQUE 1

		if swe *** BLOQUE 2 si switch es verdadero, encontro un nodo cuya columna tiene dos unos (1s)
			for i=1:length(pathaumentado)-1
				// ahora en la matriz de flujo actual quitar los unos q meti arriba
				flujo_actual(pathaumentado(i),pathaumentado(i+1))=flujo_actual(pathaumentado(i),pathaumentado(i+1))-incremento;
				flujo_actual(pathaumentado(i+1),pathaumentado(i))=flujo_actual(pathaumentado(i+1),pathaumentado(i))+incremento;
			end
			flujo_max=flujo_max-incremento; // decremento el flujo maximo
			pp=intersect(pathaumentado,posc); //pp guarda // para q no vuelva a tomar el mismo path, el pathaumentado actual, lo insecto con posc que es el q tiene las posiciones de los 1s, y veo cual es la posicion del pathaumentado con el problema
			capacidad(pp,ii)=0; // capacidad es la matriz original y en esa posicion coloco 0
		end *** FIN BLOQUE 2
		// desde aqui termina de comparar los paths
		// calcular el nuevo path aumentado
		pathaumentado = ff_pathaumentado(origen,destino,flujo_actual,capacidad,numero_nodos);
	end
end
% Determinación del corte
cf=flujo_actual;
cf(cf&lt;0)=0;
residual=capacidad-cf;
corte=cut(origen,residual,numero_nodos);

end
BLOQUE 1
El objetivo de este bloque es que si ya actualizaste la matriz de flujo actual con el pathaumentado que conseguiste, ahora revises las columnas de la matriz.
Si encuentras una columna con dos 1, quiere decir que allí estás utilizando un nodo en dos caminos. Esta búsqueda se hace hasta llegar al destino o hasta
que el swe sea true. La variable tf es un vector columna de cada posición ii hasta el destino; posc es un vector que guarda todas las posiciones de las filas
del vector columna que están en 1 en el vector tf (es decir, que si tiene más de una posición en 1, hay problemas), por esto el siguiente if comprueba si su
longitud es 2. Si es 2, se cambia el swe a true y si no, se aumenta ii

BLOQUE 2
Si el swe es true, desmonta todo el último pathaumentado que ya había sumado previamente en donde dice ACTUALIZA EL FLUJO ACTUAL CON EL
PATHAUMENTADO ENCONTRADO. Esto lo hace dentro del for i=1:length(pathaumentado)-1, luego disminuye el flujo_max de este destino en el
incremento, obtiene la intersección del pathaumentado con posc y guarda en pp. Esto lo hace porque en posc se guardan las filas del vector columna tf
que están en 1 y que una de ellas está en el último pathaumentado calculado y que está provocando que no se dé la disyunción de nodos en el
pathaumentado nuevo y alguno previo. En la matriz capacidad se asigna 0 en la posición (pp,ii) que es donde está el problema.
Luego calcula otro pathaumentado y sigue.
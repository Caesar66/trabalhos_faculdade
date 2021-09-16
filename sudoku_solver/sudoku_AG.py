from sudoku import *
import random
import sys

# Algorimto Genetico para Sudoku
#
# Representação: Lista de Posicoes
# Funcao Fitness (FF): 1/(1 + quantidade de conflitos)
# Avaliacao: FF/Soma(FF_1, + FF_2, ..., FF_n) -> A Pontuacao vai ser acumulada
# Seleção:
# Crossover: Caso haja, um numero A é escolhido e suas posições são trocadas nas configurações, movendo os outros numeros nas mesmas para as posicoes originais do numero A
# Chance: 80%
# Mutação: Escolhe duas posições quaisquer e faz um swap delas
# Chance: 1%

CHANCE_CR = 80
CHANCE_MT = 3

#Cria uma populacao inicial aleatoria
def criaPopulacao(tam_populacao, m):
	populacao = []	
	for ind in range(tam_populacao):
		populacao.append(criaConfiguracao(m))
	return populacao

def funcaoFitness(individuo, m):
	return 1/(1+avaliacao(individuo, m))

#Calucula a "pontuacao" de cada individuo e coloca numa escala de 0 a 100 (acumulado)
def pontuacao(populacao, m):
	lista_fit = []
	soma_fit = 0

	#Como usamos um algoritmo elitista, precisamos guardar o individuo de maior pontuacao
	pos_maior_ind = 0
	maior_pontuacao = 0
	#Vamos eliminar o pior individuo tambem
	pos_pior_ind = 0
	menor_pontuacao = 100	

	for ind in range(len(populacao)):
		valor_fit = funcaoFitness(populacao[ind], m)
		soma_fit = soma_fit + valor_fit
		lista_fit.append(soma_fit)
		if(valor_fit > maior_pontuacao):
			maior_pontuacao = valor_fit
			pos_melhor_ind = ind
		if(valor_fit < menor_pontuacao):
			menor_pontuacao = valor_fit
			pos_pior_ind = ind

	lista_fit[:] = [round(r*100/soma_fit) for r in lista_fit]#Colocando as pontuacao numa escala de 1 a 100
	#print(lista_fit)
	
	return lista_fit, pos_melhor_ind, pos_pior_ind

def selecao(populacao, tam_populacao, m):
	pesos, maior_ind, pior_ind = pontuacao(populacao, m)

	#Elitismo
	populacao_i = []
	populacao_i.append(populacao[maior_ind])

	#Remocao pior individuo
	pesos[pior_ind] = 0

	#Sorteio do resto da Populacao Pi'
	populacao_i_resto = random.choices(populacao, cum_weights = pesos, k = tam_populacao-1) #Escolhe a populacao Pi' baseado na pontuacao()
	populacao_i_final = populacao_i + populacao_i_resto
		
	return populacao_i_final

def crossover(individuos, m):
	gene_tipo = random.randint(1, m) #Escolhe um numero dentro para ser o que terá as posicoes trocadas
	posicoes_gene = [[],[]]
	#print(gene_tipo)
	for ind in range(len(individuos)): #Aqui guarda todas as posicoes do gene_tipo escolhido dos dois individuos
		for p_gene in range(m*m):
			if(individuos[ind][p_gene] == gene_tipo):
				posicoes_gene[ind].append(p_gene)
	#print(posicoes_gene)
	for troca in range(m): #Realiza a troca de posicoes dentro uma mesma configuracao. Ou seja: Se posicoes_gene = [[0,1,2,3],[4,5,6,7]], ele vai pegar A e vai fazer A[posicoes[0][i]], A[posicoes[1][i]] = A[posicoes[1][i]], A[posicoes[0][i]] sendo i num intervalo inteiro [0,m) 
		individuos[0][posicoes_gene[0][troca]], individuos[0][posicoes_gene[1][troca]] = individuos[0][posicoes_gene[1][troca]], individuos[0][posicoes_gene[0][troca]]
		individuos[1][posicoes_gene[0][troca]], individuos[1][posicoes_gene[1][troca]] = individuos[1][posicoes_gene[1][troca]], individuos[1][posicoes_gene[0][troca]]
	
	return individuos

def mutacao(individuo, m):
	casas = random.choices(range(m*m), k=2)
	individuo[casas[0]], individuo[casas[1]] = individuo[casas[1]], individuo[casas[0]]
	return individuo

qtde_geracoes = 100
populacao_tamanho = 6
m = 4

def algGenetico(tam_populacao, m, geracoes):
	populacao_i = criaPopulacao(tam_populacao, m)
	print("Populacao INICIAL")
	for i in range(tam_populacao):
		print(formatarSudoku(populacao_i[i], m))
		print("Solucao: " + str(avaliacao(populacao_i[i], m)))
		print("\n")

	geracao = 1
	while(geracoes > 0):
		populacao_i = selecao(populacao_i, tam_populacao, m)

		#print("Selecao: " + str(geracao))
		#for ind in populacao_i:
		#	print(formatarSudoku(ind, m))
		#	print("Solucao: " + str(avaliacao(ind, m)))
		#geracao = geracao + 1

		#Selecao de Crosspover
		pos_pop = list(range(tam_populacao)) #Aqui guardamos todas posicoes dos individuos
		while(pos_pop): 
			individuo = random.randint(0, len(pos_pop)-1)
			pos_pop.remove(pos_pop[individuo]) #Removemos o primeiro individuo 
			if(random.randint(1, 100) <= CHANCE_CR and len(pos_pop) > 0): #O ultimo elemento é submetido a vontade da populacao (se for crossover e só sobrar ele, será ele, se não for, ele será repetido)
				cross_parceiro = random.choice(pos_pop) #Escolhemos um parceiro aleatoriamente (que nao e o proprio individuo)o))
				pos_pop.remove(cross_parceiro) #Removemos o parceiro)
				descendentes = crossover([populacao_i[individuo], populacao_i[cross_parceiro]], m) #Pegamos os descendentes do crossover e colocamos no lugar dos pais
				populacao_i[individuo], populacao_i[cross_parceiro] = descendentes[0], descendentes[1]
			else:
				continue #Caso ele nao seja faça crossover, só pulamos ele (ele continua na populacao)
		#Selecao de Mutacao
		for ind in populacao_i:
			if(random.randint(1,100) <= CHANCE_MT):
				ind = mutacao(ind, m)
			else:
				continue
		geracoes = geracoes - 1
	return populacao_i

def main():
	geracoes = 1000
	tam_pop = 6
	m = int(sys.argv[1])
	populacao_final = algGenetico(tam_pop, m, geracoes)
	print("Populacao FINAL")
	for i in range(tam_pop):
		print(formatarSudoku(populacao_final[i], m))
		print("Solucao Final: " + str(avaliacao(populacao_final[i], m)))
		print("\n")

#populacao_inicial = criaPopulacao(populacao_tamanho, m)
#print(populacao_inicial)
#print(selecao(populacao_inicial, populacao_tamanho, m))

#configs = []
#test = 3
#for i in range(test):
#	configs.append(criaConfiguracao(m))
#	print(formatarSudoku(configs[i], m))
#print(pontuacao(configs, m))

#p = crossover(configs, m)
#p = mutacao(configs[0], m)
#for i in range(test):
#	print(formatarSudoku(p[i], m))

main()


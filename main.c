#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_FUNCIONARIOS 500

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int id;
    char nome[100];
    char cargo[50];
    double salarioBase;
    Data data_contratacao;
    double bonus;
    int dependentes;
    char departamento[100];
    double salarioFinal;
} Funcionario;

Funcionario funcionarios[MAX_FUNCIONARIOS];
int total_funcionarios = 0;


double salarioFinalCalc(double base, double bonus, int dependentes){
	double salario_bruto = base + bonus; 
	
//	CALCULANDO INSS 
	
	double inss = 0.0;
    double faixas[4][2] = {
        {1412, 0.075},
        {2666.68, 0.09},
        {4000.03, 0.12},
        {7786.02, 0.14}
    };
    int num_faixas = 4;
    double limite_anterior = 0.0;
    
    int i;

    for(i = 0; i < num_faixas; i++) {
        double limite = faixas[i][0];
        double aliquota = faixas[i][1];
        if(salario_bruto > limite) {
            inss += (limite - limite_anterior) * aliquota;
            limite_anterior = limite;
        } else {
            inss += (salario_bruto - limite_anterior) * aliquota;
            break;
        }
    }


    double teto_inss = 7507.49;
    if(salario_bruto > teto_inss) {
        inss = 7507.49 * 0.14;
    }
    
    
//    CALCULANDO IRFF
    
    double base_calculo = salario_bruto - inss;
    
    double deducao_dependente = 189.59;
    base_calculo -= (dependentes * deducao_dependente);

    double irrf = 0.0;

    if(base_calculo <= 2259.20) {
        irrf = 0.0;
    }
    else if(base_calculo <= 2826.65) {
        irrf = (base_calculo * 0.075) - 169.44;
    }
    else if(base_calculo <= 3751.05) {
        irrf = (base_calculo * 0.15) - 381.44;
    }
    else if(base_calculo <= 4664.68) {
        irrf = (base_calculo * 0.225) - 662.77;
    }
    else {
        irrf = (base_calculo * 0.275) - 896.00;
    }
    
    
    if(irrf < 0) {
        irrf = 0.0;
    }
    
    double salario_liquido = salario_bruto - inss - irrf;
    
    return salario_liquido; 

	
}

void cadastrar_funcionario() {
    char confirmacao;
    Funcionario novo_funcionario;
    char data_input[11];

    printf("\nDigite o ID do funcionário: ");
    scanf("%d", &novo_funcionario.id);

    getchar();  // Limpar o buffer do teclado

    printf("Digite o nome do funcionário: ");
    fgets(novo_funcionario.nome, 100, stdin);
    novo_funcionario.nome[strcspn(novo_funcionario.nome, "\n")] = '\0';

    printf("Digite o cargo do funcionário: ");
    fgets(novo_funcionario.cargo, 50, stdin);
    novo_funcionario.cargo[strcspn(novo_funcionario.cargo, "\n")] = '\0'; 

    do {
        printf("Digite a data de contratação no formato (dd/mm/aaaa): ");
        fgets(data_input, 11, stdin);

        if (sscanf(data_input, "%d/%d/%d", &novo_funcionario.data_contratacao.dia, &novo_funcionario.data_contratacao.mes, &novo_funcionario.data_contratacao.ano) != 3) {
            printf("Data inválida! Use o formato dd/mm/aaaa.\n");
        }
    } while (sscanf(data_input, "%d/%d/%d", &novo_funcionario.data_contratacao.dia, &novo_funcionario.data_contratacao.mes, &novo_funcionario.data_contratacao.ano) != 3);
    
    
    do{
    	printf("Digite o salário base do funcionário: ");
    	scanf("%lf", &novo_funcionario.salarioBase);
    	
    	if(novo_funcionario.salarioBase < 0){
    		printf("Salário Base inválido! Digite um valor positivo.\n");	
		};
	}while(novo_funcionario.salarioBase < 0);
    
    
    do{
    	printf("Digite o bonûs do funcionário (caso não possua bonûs digite: 0): ");
    	scanf("%lf", &novo_funcionario.bonus);
    	
    	if(novo_funcionario.bonus < 0){
    		printf("Salário bonûs inválido! Digite um valor positivo.\n");	
		};
	}while(novo_funcionario.bonus < 0);
	
	
	do{
    	printf("Digite a quantidade de dependentes do funcionario (caso não possua dependentes digite: 0): ");
    	scanf("%d", &novo_funcionario.dependentes);
    	
    	if(novo_funcionario.dependentes < 0){
    		printf("Número de dependentes inválido! Digite um valor positivo.\n");	
		};
	}while(novo_funcionario.dependentes < 0);
	
	getchar();
	
	printf("Digite o departamento ao qual o funcionário pertence: ");
    fgets(novo_funcionario.departamento, 100, stdin);
    novo_funcionario.departamento[strcspn(novo_funcionario.departamento, "\n")] = '\0';
    
    novo_funcionario.salarioFinal = salarioFinalCalc(novo_funcionario.salarioBase,novo_funcionario.bonus,novo_funcionario.dependentes);
    
    printf("\nDeseja realmente cadastrar o funcionário? (S/N): ");  
    scanf(" %c", &confirmacao);

    if (confirmacao == 'S' || confirmacao == 's') {
        funcionarios[total_funcionarios] = novo_funcionario;
        total_funcionarios++;
        printf("\nFuncionário cadastrado com sucesso!\n");
    } else {
        printf("\nCadastro cancelado!\n");
    }
}

void listar_funcionarios() {
    if (total_funcionarios == 0) {
        printf("\nNenhum funcionário cadastrado.\n");
    } else {
        printf("\n\n========= Lista de Funcionários Cadastrados =========\n");
        int i;
        for (i = 0; i < total_funcionarios; i++) {
            printf("\nFuncionário %d:\n", i + 1);
            printf("ID: %d\n", funcionarios[i].id);
            printf("Nome: %s\n", funcionarios[i].nome);
            printf("Cargo: %s\n", funcionarios[i].cargo);
            printf("Data de contratação: %02d/%02d/%04d\n", funcionarios[i].data_contratacao.dia, funcionarios[i].data_contratacao.mes, funcionarios[i].data_contratacao.ano);
            printf("Salário base: %.2lf\n", funcionarios[i].salarioBase);
            printf("Bonûs: %.2lf\n", funcionarios[i].bonus);
            printf("Dependentes: %d\n", funcionarios[i].dependentes);
            printf("Departamento: %s\n", funcionarios[i].departamento);
            printf("Salário final: %.2lf\n", funcionarios[i].salarioFinal);
        }
        printf("\n=================================================\n");
    }
}

void editarFuncionario(){
	int idFuncionario;
	int i;
	
	printf("\n\n========= Editando Funcionário =========\n\n");
	printf("Digite o ID de um funcionário já cadastrado:");
	scanf("%d",&idFuncionario);
	
	getchar();
	
	
	for(i = 0; i < total_funcionarios; i++) {
        if(funcionarios[i].id == idFuncionario){
        	char confirmacao;
		    Funcionario novo_funcionario;
		
			novo_funcionario.id = funcionarios[i].id;
			strcpy(novo_funcionario.nome, funcionarios[i].nome);
			novo_funcionario.data_contratacao = funcionarios[i].data_contratacao;
		
		    printf("Digite o cargo do funcionário: ");
		    fgets(novo_funcionario.cargo, 50, stdin);
		    novo_funcionario.cargo[strcspn(novo_funcionario.cargo, "\n")] = '\0'; 
		    
		    
		    do{
		    	printf("Digite o salário base do funcionário: ");
		    	scanf("%lf", &novo_funcionario.salarioBase);
		    	
		    	if(novo_funcionario.salarioBase < 0){
		    		printf("Salário Base inválido! Digite um valor positivo.\n");	
				};
			}while(novo_funcionario.salarioBase < 0);
		    
		    
		    do{
		    	printf("Digite o bonûs do funcionário (caso não possua bonûs digite: 0): ");
		    	scanf("%lf", &novo_funcionario.bonus);
		    	
		    	if(novo_funcionario.bonus < 0){
		    		printf("Salário bonûs inválido! Digite um valor positivo.\n");	
				};
			}while(novo_funcionario.bonus < 0);
			
			
			do{
		    	printf("Digite a quantidade de dependentes do funcionario (caso não possua dependentes digite: 0): ");
		    	scanf("%d", &novo_funcionario.dependentes);
		    	
		    	if(novo_funcionario.dependentes < 0){
		    		printf("Número de dependentes inválido! Digite um valor positivo.\n");	
				};
			}while(novo_funcionario.dependentes < 0);
			
			getchar();
			
			printf("Digite o departamento ao qual o funcionário pertence: ");
		    fgets(novo_funcionario.departamento, 100, stdin);
		    novo_funcionario.departamento[strcspn(novo_funcionario.departamento, "\n")] = '\0';
		    
		    novo_funcionario.salarioFinal = salarioFinalCalc(novo_funcionario.salarioBase,novo_funcionario.bonus,novo_funcionario.dependentes);
		    
		    printf("\nDeseja realmente [editar] o funcionário? (S/N): ");  
		    scanf(" %c", &confirmacao);
		
		    if (confirmacao == 'S' || confirmacao == 's') {
		        funcionarios[i] = novo_funcionario;
		        printf("\nFuncionário editado com sucesso!\n");
		    } else {
		        printf("\nEdição cancelada!\n");
		    }
		}
    };	
	
};

void gerarFolhaPagamento() {
	
    if (total_funcionarios == 0) {
	    printf("\nNenhum funcionário cadastrado para gerar o relatório.\n");
	    return;
    }
    
    int i;
    double salario_mensal;
    
    for(i = 0; i < total_funcionarios; i++) {
        salario_mensal += (funcionarios[i].salarioBase + funcionarios[i].bonus);
    }
    
    printf("Relatório de Folha de Pagamento: O total de salários a serem pagos pela empresa no mês é: %.2lf\n",salario_mensal);
}


void deletarFuncionario() {
    int idFuncionario;
    int i, j;
    int encontrado = 0;

    printf("\n\n========= Deletando Funcionário =========\n\n");
    printf("Digite o ID do funcionário que deseja deletar: ");
    scanf("%d", &idFuncionario);

    
    for(i = 0; i < total_funcionarios; i++) {
        if(funcionarios[i].id == idFuncionario) {
            encontrado = 1;
            
            char confirmacao;
            printf("Funcionário encontrado:\n");
            printf("ID: %d\n", funcionarios[i].id);
            printf("Nome: %s\n", funcionarios[i].nome);
            printf("Deseja realmente deletar este funcionário? (S/N): ");
            scanf(" %c", &confirmacao);
            
            if(confirmacao == 'S' || confirmacao == 's') {
                
                for(j = i; j < total_funcionarios - 1; j++) {
                    funcionarios[j] = funcionarios[j + 1];
                }
                total_funcionarios--;
                printf("\nFuncionário deletado com sucesso!\n");
            } else {
                printf("\nDeletar funcionário cancelado!\n");
            }
            break; 
        }
    }

    if(!encontrado) {
        printf("\nFuncionário com ID %d não encontrado.\n", idFuncionario);
    }
}


// ================================================================================================= ||

void relatorio_por_departamento() {
    if (total_funcionarios == 0) {
        printf("\nNenhum funcionário cadastrado para gerar o relatório.\n");
        return;
    }

    printf("\n\n========= Relatório de Funcionários por Departamento =========\n");

    // Armazenar os totais por departamento
    double total_gasto[MAX_FUNCIONARIOS] = {0.0};
    int departamentos_contados = 0;
    char departamentos[MAX_FUNCIONARIOS][100];
	
	int i;
    // Iterar sobre os funcionários e calcular os valores por departamento
    for (i = 0; i < total_funcionarios; i++) {
        int encontrado = -1;
	
	
		int j;
        // Verificar se o departamento já foi contado
        for (j = 0; j < departamentos_contados; j++) {
            if (strcmp(departamentos[j], funcionarios[i].departamento) == 0) {
                encontrado = j;
                break;
            }
        }

        if (encontrado == -1) {
            // Adicionar novo departamento
            strcpy(departamentos[departamentos_contados], funcionarios[i].departamento);
            encontrado = departamentos_contados;
            departamentos_contados++;
        }

        // Adicionar o salário do funcionário ao total do departamento
        total_gasto[encontrado] += funcionarios[i].salarioFinal;
    }

    // Mostrar o relatório por departamento
    for ( i = 0; i < departamentos_contados; i++) {
        printf("\nDepartamento: %s\n", departamentos[i]);
        printf("Funcionários:\n");
        
        int j;

        for (j = 0; j < total_funcionarios; j++) {
            if (strcmp(funcionarios[j].departamento, departamentos[i]) == 0) {
                printf(" - %s (ID: %d) - Salário Final: %.2lf\n", funcionarios[j].nome, funcionarios[j].id, funcionarios[j].salarioFinal);
            }
        }

        printf("Total gasto em salários no departamento: %.2lf\n", total_gasto[i]);
    }

    printf("\n=============================================================\n");
}

// ================================================================================================= ||

int main() {
    setlocale(LC_ALL, "Portuguese");
    char continuar_cadastro;
    int opcao = 0;

    printf("=== Sistema de Gestão de Funcionários ===\n");

    do {
        cadastrar_funcionario();

        printf("\nDeseja cadastrar outro funcionário? (S/N): ");  
        scanf(" %c", &continuar_cadastro);

    } while (continuar_cadastro == 'S' || continuar_cadastro == 's');

    listar_funcionarios();
    
    do{
    	printf("\nQual ação deseja executar em seguida? :\n");
        printf("\n1 - Editar funcionário\n");
        printf("2 - Listar funcionários ativos\n");
        printf("3 - Gerar Folha de Pagamento\n");
        printf("4 - Gerar Relatório por Departamento\n");
        printf("5 - Deletar funcionário\n");
        printf("0 - Finalizar o programa\n");
        printf("\nDigite a opção desejada: ");
        scanf("%d",&opcao);
    	
    	if(opcao == 1){
    		editarFuncionario();
		}else if(opcao == 2){
			listar_funcionarios();
		}else if (opcao == 3){
			gerarFolhaPagamento();	
		}else if(opcao == 4){
			relatorio_por_departamento();
		}else if(opcao == 5){
			deletarFuncionario();
		}
	}while(opcao != 0);
    

    printf("\nPrograma encerrado.\n");

    return 0;
}


#include <stdio.h>
#include <string.h>

void exibir_menu_emoticons() {
    printf("\nSelecione um emoticon:\n");
    printf("1. :-) (sorriso)\n");
    printf("2. :-( (triste)\n");
    printf("3. ;-) (piscada)\n");
    printf("4. :-D (riso)\n");
    printf("5. :-O (surpreso)\n");
    printf("6. :-P (lingua)\n");
    printf("7. nenhum\n");
    printf("Escolha uma opção (1 a 7): ");
}

char* obter_emoticon(int escolha) {
    switch (escolha) {
        case 1: return ":-)";
        case 2: return ":-(";
        case 3: return ";-)";
        case 4: return ":-D";
        case 5: return ":-O";
        case 6: return ":-P";
        default: return "";
    }
}

int main() {
    char mensagem[100];
    char buffer[100];
    char *emoticon = "";
    int escolha_emoticon;

    while(1) {
        printf("Digite '/emoticons' para selecionar um emoticon, ou digite sua mensagem: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if(strcmp(buffer, "/emoticons") == 0) {
            exibir_menu_emoticons();
            scanf("%d", &escolha_emoticon);
            while(getchar() != '\n');
            emoticon = obter_emoticon(escolha_emoticon);
            printf("Emoticon selecionado: %s\n", emoticon);
        } else {
            strcpy(mensagem, buffer);
            if (strlen(emoticon) > 0) {
                strcat(mensagem, " ");
                strcat(mensagem, emoticon);
            }
            
            printf("Você: %s\n", mensagem);
            emoticon = "";
        }
    }
    return 0;
}

============================================================================

#include <stdio.h>
#include <string.h>

void imprimeicone(char* n);

int main(){
	char str[] = "1F600";
	imprimeicone(str);
	return 0;
}

void imprimeicone(char* n){
	char str[1000];
        sprintf(str,"powershell.exe -Command \"[System.Char]::ConvertFromUtf32([System.Convert]::toInt32('%s',16))\"", n);
	system(str);
}
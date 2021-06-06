/**
 * @file       tree.c
 * @author     Ondřej Ševčík
 * @date       6/2019
 * @brief      Source file for binary tree
 * **********************************************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

#include "tree.h"
#include "mymalloc.h"

/* Local functions declaration ---------------------------------------------- */
/**
 * @brief print_postorder   Recursive function that calls itself with the left
 * node and then right node and then processes node that is called with the
 * function that is passed in argument
 * @param node  Pointer at node
 * @param proc  function that is called by each node
 */
void print_postorder(TreeNode* node, TreeNodeProc proc);

/**
 * @brief print_inorder Recursive function that calls itself with the left node
 * first, then processes the current node with the function that is passed in
 * argument and then it calls itself with the right node
 * @param node  Pointer at node
 * @param proc  function that is called by each node
 */
void print_inorder(TreeNode* node, TreeNodeProc proc);

/**
 * @brief print_preorder    Recursive function that processes node first, then
 * it calls itself with the left and then right node
 * @param node  Pointer at node
 * @param proc  function that is called by each node
 */
void print_preorder(TreeNode* node, TreeNodeProc proc);

bool Tree_Init(Tree* const root)
{
    //Celkem špatně nazvaná proměnná "root" a pak máme "root->root?"
    //Ošetření vstupů
    if(root == NULL)
    {
       return false;
    }
    else
    {
        //Tu kořenovou hodnotu "uzel" nastavíme na NULL
        root->root = NULL;
        //Počet hodnot ve stromu bude nula
        root->itemsCount = 0;
        //Na konci vrátíme true;
        return true;
    }
}
void Tree_Rekursive_Clear(TreeNode* node)
{
    //Pokud by byla node NULL tak nemáme co řešit
    if (node == NULL)
    {
        return;
    }
    if(node->left != NULL)
    {
        //Smažame levý uzel
        Tree_Rekursive_Clear(node->left);
    }
    if(node->right != NULL)
    {
        //Smažeme pravý uzel
        Tree_Rekursive_Clear(node->right);
    }
    //Uvolníme paměť uzlu
    myFree(node);
    //Nastavíme uzel na NULL
    node = NULL;
}
void Tree_Clear(Tree* const root)
{
    if(root == NULL || root->root == NULL)
    {
        return;
    }
    //Rekurzivním voláním dealokujeme paměť (uvolníme) ve všech uzlech
    Tree_Rekursive_Clear(root->root);
    //Po smazání všech levých a pravých uzlů nastavíme hlavní uzel "root" na NULL
    root->root = NULL;
    //Počet prvků nastavíme na nula. jelikož jsme všechyn smazaly (uvolnily)
    root->itemsCount = 0;

}

bool Tree_Insert(Tree* const root, const Data_t data)
{
    if(root == NULL)
    {
        return false;
    }
    //Naalokujeme paměť pro nový uzel
    TreeNode* tnode = myMalloc(sizeof (TreeNode));
    //Ověříme zda se paměť alokovala správně
    if(tnode == NULL)
    {
        return false;
    }
    //Vložíme do uzlu data
    tnode->data = data;

    //Nastavíme ukazetele na ty nové listy na NULL
    tnode->left = NULL;
    tnode->right = NULL;

    //Pokud je to úplně první prvek, tak je to úplně nejjednodušší
    //Dám ho do toho kořene stromu
    if(root->itemsCount == 0)
    {
        //Do rootu přidáme tnode
        root->root = tnode;
        root->itemsCount++;
        return true;
    }
    else
    {
        //Když máme více prvků a musíme se rozhodnout kde přidáme nový uzel
        //uděláme si pomocnou proměnnou, rozhodneme se, kterým směrem toho
        //stromu pak půjdeme (left or right)
        //Root existuje (není itemsCount =0)

        TreeNode* next = root->root;
        TreeNode* last = NULL;
        //Pokud výsledek Cmp bude nula, tak ty pdaje v tom stromě už jednou jsou
        //Tím pádem bychom měli vrátit false
        //Pokud to bude menší jak nula, tak jdu na pravo, pokud větší tak nalevo
        int cmp;
        while(next != NULL)
        {
            cmp = Data_Cmp(&next->data, &data);
            if(cmp == 0)
            {
                //Dealokujeme naalokovanou paměť
                myFree(tnode);
                //Přidali bychom co tam už je -> vracíme false;
                return false;
            }
            if(cmp < 0)
            {
                last = next;
                next = next->right;
            }
            else if(cmp > 0)
            {
                last = next;
                next = next->left;
            }
        }
        //Přidáme uzel
        if(cmp < 0)
        {
            last->right = tnode;
        }
        else if(cmp > 0)
        {
            last->left = tnode;
        }
        root->itemsCount++;
        return true;
    }
}

void Tree_Delete(Tree* const root, const Data_t data)
{
    //Ošetření vstupu
    if(root == NULL || root->root == NULL)
    {
        return;
    }

    TreeNode* findNode = root->root;
    TreeNode** lastNode = NULL;
    TreeNode* node = NULL;

    while(findNode != NULL)
    {
        int cmp = Data_Cmp(&findNode->data, &data);

        if(cmp < 0)
        {
          lastNode = &findNode->right;
          findNode = findNode->right;
        }
        else if(cmp > 0)
        {
            lastNode = &findNode->left;
            findNode = findNode->left;
        }
        else if(cmp == 0)
        {

            //Obě child existují, přepíšeme rodiče s největším left child
            if(findNode->left != NULL && findNode->right != NULL)
            {

                TreeNode* maxNode = findNode->left;
                TreeNode* tempPrevNode = NULL;

                while(maxNode->right != NULL)
                {
                    tempPrevNode = maxNode;
                    maxNode = maxNode->right;
                }

                findNode->data = maxNode->data;

                if(tempPrevNode != NULL)
                {
                  tempPrevNode->right = maxNode->left;
                }

                else
                {
                  findNode->left = maxNode->left;
                }
                myFree(maxNode);
                root->itemsCount--;
                return;
            }

            //Nemáme žádné child
            else if(findNode->right == NULL && findNode->left == NULL)
            {
                root->itemsCount--;
                *lastNode = NULL;
                myFree(findNode);
                return;
            }

            //Máme left nebo right child
            else
            {

                if(findNode->left != NULL)
                {
                  node = findNode->left;

                  findNode->data = findNode->left->data;
                  findNode->right = findNode->left->right;
                  findNode->left = findNode->left->left;



                  root->itemsCount--;
                  myFree(node);
                  return;
                }
                else
                {
                  node = findNode->right;

                  findNode->data = node->right->data;
                  findNode->right = findNode->right->right;
                  findNode->left = findNode->right->left;



                  root->itemsCount--;
                  myFree(node);
                  return;
                }
            }
        }
    }
}

const Data_t* Tree_Get_Data(const TreeNode* const node)
{
    //Ošetříme vstup, že není NULL
    if(node == NULL)
    {
        return NULL;
    }
    //Vrátíme ukazatel na data z node
    return &node->data;
}

TreeNode* Tree_Find_Node(Tree root, const Data_t data)
{
    if(root.root == NULL || root.itemsCount == 0)
    {
        return NULL;
    }
    TreeNode* findNode= root.root;
    while(findNode != NULL)
    {
        //Porovnáváme data v node s těmi uživatelskými
        int cmp = Data_Cmp(&findNode->data, &data);
        if (cmp == 0)
        {
            // Cmp -> 0=0 ->našli jsme hledanou node
            return findNode;
        }
        else if (cmp < 0)
        {
            //Hledáme "naši" node v levém uzlu
            findNode = findNode->right;
        }
        else if(cmp > 0)
        {
            //Hledáme "naši" node v pravém užlu
            findNode = findNode->left;
        }
    }
    //Pokud node dle data nejajdeme vracíme NULL
    return NULL;
}

size_t Tree_Get_Count(Tree root)
{
    //Vrácíme počet itemCount
    return root.itemsCount;
}

void Tree_Process(Tree root, TreeNodeProc proc, TreeProcessMode mode)
{
    //tři vstupní parametry (strom, proc- funkce, která tiskne jednotlivé uzly, mod- v jakém pořadí se ty uzly tisknou na konzoli)
    //Bude rozhodovat, která z funkcí se bude spouštět
    switch (mode)
    {
        case procPREORDER:
            print_preorder(root.root, proc);
        break;

        case procINORDER:
            print_inorder(root.root, proc);
        break;

        case procPOSTORDER:
            print_postorder(root.root, proc);
        break;

    }
}

void print_preorder(TreeNode* node, TreeNodeProc proc)
{
    //Nejdřív se zpracuje (vytiksne) ten konrkétní uzel na kterém pracujeme,pak se přensuneme na ty další (levý a pravý)
    if(node != NULL)
    {
        // 1) uzel, 2)levá, 3)pravá
        (*proc)(node);
        print_preorder(node->left, proc);
        print_preorder(node->right, proc);
    }
}

void print_inorder(TreeNode* node, TreeNodeProc proc)
{
    //Vytiskneme levého potomka, následně smaotný uzol a pak pravého potomka
    if(node != NULL)
    {
        // 1)levá, 2) uzel, 3)pravá
        print_inorder(node->left, proc);
        (*proc)(node);
        print_inorder(node->right, proc);
    }
}

void print_postorder(TreeNode* node, TreeNodeProc proc)
{
    //nejdřív oba dva potomky (levý, pravý a pak samotný uzel)
    if(node != NULL)
    {
        // 1)levá, 2)pravá, 3)uzel
        print_postorder(node->left, proc);
        print_postorder(node->right, proc);
        (*proc)(node);
    }
}

void Tree_Print(TreeNode* node, TreeNode* previous, int spaces) {
  (void)node;
  (void)previous;
  (void)spaces;
}

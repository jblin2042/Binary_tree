#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#define D_array_max        50000
#define MAX_LAST_NAME_SIZE 16

typedef struct _Phone_info {
	char LastName [ MAX_LAST_NAME_SIZE + 1 ] ;
	char FirstName [ 16 ] ;
	char email [ 16 ] ;
	char phone [ 10 ] ;
	char cell [ 10 ] ;
	char addr1 [ 16 ] ;
	char addr2 [ 16 ] ;
	char city [ 16 ] ;
	char state [ 2 ] ;
	char zip [ 5 ] ;
} Phone_info ;

typedef struct __PHONE_BOOK_ENTRY {
	Phone_info *info ;
	struct __PHONE_BOOK_ENTRY *next_L ;
	struct __PHONE_BOOK_ENTRY *next_R ;
} PhoneBook ;

void Clear_list ( PhoneBook **root ) ;


PhoneBook *Create_node ( Phone_info *info ) {
	if ( ! info ) {
		return NULL ;
	}

	PhoneBook* p_tmp = ( PhoneBook* ) calloc ( 1 , sizeof(PhoneBook) ) ;
	if ( ! p_tmp ) {
		return NULL ;
	}

	p_tmp->info = ( Phone_info* ) calloc ( 1 , sizeof(Phone_info) ) ;
	if ( ! p_tmp->info ) {
		free ( p_tmp ) ;
		p_tmp = NULL ;
		return NULL ;
	}
	memcpy ( p_tmp->info , info , sizeof(Phone_info) ) ;
	p_tmp->next_L = NULL ;
	p_tmp->next_R = NULL ;
	return p_tmp ;
}

int Inser_to_tree ( PhoneBook** root , PhoneBook* p_node ) {
	if ( ! p_node )
		return - 1 ;

	if ( ! * root ) {
		* root = p_node ;
		return 1 ;
	}

	int rtn = 0 ;
	PhoneBook* p_root = * root ;
	PhoneBook** p_addr = NULL ;
	while ( 1 ) {
		rtn = memcmp ( p_node->info , p_root->info , sizeof(PhoneBook) ) ;
		p_addr = & p_root->next_R ;
		if ( 0 >= rtn ) {
			p_addr = & p_root->next_L ;
		}
		if ( ! * p_addr ) {
			* p_addr = p_node ;
			break ;
		}
		p_root = * p_addr ;
	}
	return 1 ;
}

int Make_binary_tree ( PhoneBook** root , Phone_info info [ D_array_max ] ) {
	PhoneBook* p_tmp = NULL ;
	int i = 0 ;
	for ( ; i < D_array_max ; ++ i ) {
		p_tmp = Create_node ( info + i ) ;
		if ( NULL == p_tmp ) {
			Clear_list ( root ) ;
			return - 2 ;
		}
		int rtn = Inser_to_tree ( root , p_tmp ) ;
		if ( 1 != rtn ) {
			Clear_list ( root ) ;
			return - 3 ;
		}
	}
	return 1 ;
}

// 建立亂數資料 //
void Make_rand ( Phone_info info [ D_array_max ] ) {
	char base_char [ 26 ] = { 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' } ;
	srand ( time ( NULL ) ) ;
	int i = 0 ;
	int j = 0 ;
	for ( i = 0 ; i < D_array_max ; ++ i ) {
		for ( j = 0 ; j < MAX_LAST_NAME_SIZE ; ++ j ) {
			info [ i ].LastName [ j ] = base_char [ rand ( ) % sizeof ( base_char ) ] ;
		}
		info [ i ].LastName [ MAX_LAST_NAME_SIZE ] = 0 ;
	}
	return ;
}


PhoneBook* Search_last_name ( PhoneBook* root , char* LastName ) {
	int rtn ;
	int cmp_sz = MAX_LAST_NAME_SIZE + 1 ;
	PhoneBook** p_addr = NULL ;
	while ( root ) {
		rtn = memcmp ( LastName , root->info->LastName , cmp_sz ) ;
		p_addr = & root->next_R ;
		if ( 0 == rtn ) {
			return root ;
		} else if ( 0 > rtn ) {
			p_addr = & root->next_L ;
		}
		root = * p_addr ;
	}
	return NULL ;
}

void Show_preorder ( PhoneBook* root ) {
	if ( ! root ) {
		printf ( "\n" ) ;
		return ;
	}
	printf ( "%s\n" , root->info->LastName ) ;
	Show_preorder ( root->next_L ) ;
	Show_preorder ( root->next_R ) ;
	return ;
}
void Show_inorder ( PhoneBook* root ) {
	if ( ! root ) {
		printf ( "\n" ) ;
		return ;
	}
	Show_inorder ( root->next_L ) ;
	printf ( "%s\n" , root->info->LastName ) ;
	Show_inorder ( root->next_R ) ;
	return ;
}
void Show_postorder ( PhoneBook* root ) {
	if ( ! root ) {
		printf ( "\n" ) ;
		return ;
	}
	Show_postorder ( root->next_L ) ;
	Show_postorder ( root->next_R ) ;
	printf ( "%s\n" , root->info->LastName ) ;
	return ;
}

//void Clear_list ( PhoneBook **root ) {
//	if ( ! ( * root ) ) {
//		return ;
//	}
//	Clear_list ( & ( * root )->next_L ) ;
//	free ( ( * root )->next_L->info ) ;
//	( * root )->next_L->info = NULL ;
//
//	free ( ( * root )->next_L ) ;
//	( * root )->next_L = NULL ;
//
//	Clear_list ( & ( * root )->next_R ) ;
//	free ( ( * root )->next_R->info ) ;
//	( * root )->next_R->info = NULL ;
//
//	free ( ( * root )->next_R ) ;
//	( * root )->next_R = NULL ;
//
//	free ( * root ) ;
//	* root = NULL ;
//
//}
// 計時器 //
struct timeval g_timer_begin ;
struct timeval g_timer_end ;
void Begin_time ( void ) {
	gettimeofday ( & g_timer_begin , NULL ) ;
	return ;
}

long double End_time ( void ) {
	gettimeofday ( & g_timer_end , NULL ) ;
	long int d_s = g_timer_end.tv_sec - g_timer_begin.tv_sec ;
	long int d_us = g_timer_end.tv_usec - g_timer_begin.tv_usec ;
	if ( g_timer_end.tv_usec < g_timer_begin.tv_usec ) {
		-- d_s ;
		d_us = ( 1000000 - g_timer_begin.tv_usec ) + g_timer_end.tv_usec ;
	}
	return d_s + ( ( long double ) d_us / 10e5 ) ;
}

char g_buf [ 8912 ] = { 0 } ;
int g_sz = 0 ;
void Printf ( const char* fmt , ... ) {
	va_list ap ;
	va_start ( ap , fmt ) ;
	int rtn = vsnprintf ( g_buf + g_sz , sizeof ( g_buf ) - g_sz , fmt , ap ) ;
	va_end ( ap ) ;
	g_sz += rtn ;
	if ( 8900 < g_sz ) {
		g_sz = 0 ;
		printf ( "%s" , g_buf ) ;
		g_buf [ 0 ] = 0 ;
	}
	return ;
}


int main ( void ) {
	// 建立亂數資料 ( 測試用 ) //
	Phone_info info [ D_array_max ] ;
	Make_rand ( info ) ;

	// 建立二元樹 //
	PhoneBook* root = NULL ;
	Printf ( "Test data sz:%d\n" , D_array_max ) ;
	Begin_time ( ) ;
	Make_binary_tree ( & root , info ) ;
//	Show_preorder ( root ) ;
	Printf ( "Make binary tree time(us):%Lf\n" , End_time ( ) ) ;

	// 開始將資料搜尋 ( 測試用 ) //
	Begin_time ( ) ;
	int i = 0 ;
	for ( i = 0 ; i < D_array_max ; ++ i ) {
		PhoneBook* p_search = Search_last_name ( root , info [ i ].LastName ) ;
		if ( ! p_search ) {
			Printf ( "Last name:%s not find\n" , info [ i ].LastName ) ;
		}
	}
	Printf ( "Sear all data time(us):%Lf\n" , End_time ( ) ) ;
	printf ( "%s" , g_buf ) ;
	return 0 ;
}


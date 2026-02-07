/* Simple C trie-tree library
 *
 * MIT License
 *
 * Copyright (c) 2025 at2er <xb0515@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */
#ifndef SCTRIE_H
#define SCTRIE_H
#include <limits.h>
#include <stdlib.h>

struct sctrie__tree_node_t;
struct sctrie__tree_node_t {
	struct sctrie__tree_node_t *nodes[UCHAR_MAX + 1];
	void *data;
};

extern void *sctrie_find_elem(const void *tree, const char *str, int str_len);
extern void *sctrie_append_elem(void *tree, size_t node_size,
		const char *str, int str_len);
extern void *sctrie_append_or_find_elem(void *tree, size_t node_size,
		const char *str, int str_len);
extern void sctrie_free_node(void *tree, void (*node_free)(void *node));
extern void sctrie_free_tree(void *tree, void (*node_free)(void *node));
extern void sctrie_free_tree_noself(void *tree, void (*node_free)(void *node));
#endif /* SCTRIE_H */

#ifdef SCTRIE_IMPL
#include <stdint.h>
#include <stdlib.h>

void *sctrie_find_elem(const void *tree, const char *str, int str_len)
{
	struct sctrie__tree_node_t *result = (struct sctrie__tree_node_t*)tree;
	for (int i = 0; i < str_len; i++) {
		if (result->nodes[(uint8_t)str[i]] == NULL)
			return NULL;
		result = result->nodes[(uint8_t)str[i]];
	}
	return result;
}

void *sctrie_append_elem(void *tree, size_t node_size,
		const char *str, int str_len)
{
	struct sctrie__tree_node_t *cur = (struct sctrie__tree_node_t*)tree;
	for (int i = 0; i < str_len; i++) {
		if (cur->nodes[(uint8_t)str[i]] != NULL) {
			if (i + 1 == str_len)
				return NULL;
			cur = cur->nodes[(uint8_t)str[i]];
			continue;
		}
		cur->nodes[(uint8_t)str[i]] = calloc(1, node_size);
		cur = cur->nodes[(uint8_t)str[i]];
	}
	return cur;
}

void *sctrie_append_or_find_elem(void *tree, size_t node_size,
		const char *str, int str_len)
{
	struct sctrie__tree_node_t *cur = (struct sctrie__tree_node_t*)tree;
	for (int i = 0; i < str_len; i++) {
		if (cur->nodes[(uint8_t)str[i]] == NULL)
			cur->nodes[(uint8_t)str[i]] = calloc(1, node_size);
		cur = cur->nodes[(uint8_t)str[i]];
	}
	return cur;
}

void sctrie_free_node(void *tree, void (*node_free)(void *node))
{
	struct sctrie__tree_node_t *cur = (struct sctrie__tree_node_t*)tree;
	for (int i = 0; i < UCHAR_MAX; i++) {
		if (cur->nodes[i] == NULL)
			continue;
		sctrie_free_node(cur->nodes[i], node_free);
	}
	node_free(cur);
}

void sctrie_free_tree_noself(void *tree, void (*node_free)(void *node))
{
	struct sctrie__tree_node_t *cur = (struct sctrie__tree_node_t*)tree;
	for (int i = 0; i < UCHAR_MAX; i++) {
		if (cur->nodes[i] == NULL)
			continue;
		sctrie_free_node(cur->nodes[i], node_free);
	}
}

void sctrie_free_tree(void *tree, void (*node_free)(void *node))
{
	sctrie_free_tree_noself(tree, node_free);
	node_free(tree);
}
#endif /* SCTRIE_IMPL */

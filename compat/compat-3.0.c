/*
 * Copyright 2011    Hauke Mehrtens <hauke@hauke-m.de>
 * Copyright 2011    Alexey Dobriyan <adobriyan@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Compatibility file for Linux wireless for kernels 3.0.
 */

#include <linux/compat.h>
#include <linux/if_ether.h>

/* This pulls-in a lot of non-exported symbol backports
 * on kernels older than 2.6.32. There's no harm for not
 * making this available on kernels < 2.6.32. */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#include <linux/pagemap.h>

/* This backports:
 *
 * commit d9d90e5eb70e09903dadff42099b6c948f814050
 * Author: Hugh Dickins <hughd@google.com>
 * Date:   Mon Jun 27 16:18:04 2011 -0700
 *
 *	tmpfs: add shmem_read_mapping_page_gfp
 */

struct page *shmem_read_mapping_page_gfp(struct address_space *mapping,
                                        pgoff_t index, gfp_t gfp)
{
       return read_cache_page_gfp(mapping, index, gfp);
}
EXPORT_SYMBOL_GPL(shmem_read_mapping_page_gfp);
#endif

int mac_pton(const char *s, u8 *mac)
{
	int i;

	/* XX:XX:XX:XX:XX:XX */
	if (strlen(s) < 3 * ETH_ALEN - 1)
		return 0;

	/* Don't dirty result unless string is valid MAC. */
	for (i = 0; i < ETH_ALEN; i++) {
		if (!strchr("0123456789abcdefABCDEF", s[i * 3]))
			return 0;
		if (!strchr("0123456789abcdefABCDEF", s[i * 3 + 1]))
			return 0;
		if (i != ETH_ALEN - 1 && s[i * 3 + 2] != ':')
			return 0;
	}
	for (i = 0; i < ETH_ALEN; i++) {
		mac[i] = (hex_to_bin(s[i * 3]) << 4) | hex_to_bin(s[i * 3 + 1]);
	}
	return 1;
}
EXPORT_SYMBOL_GPL(mac_pton);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32))

#define kstrto_from_user(f, g, type)					\
int f(const char __user *s, size_t count, unsigned int base, type *res)	\
{									\
	/* sign, base 2 representation, newline, terminator */		\
	char buf[1 + sizeof(type) * 8 + 1 + 1];				\
									\
	count = min(count, sizeof(buf) - 1);				\
	if (copy_from_user(buf, s, count))				\
		return -EFAULT;						\
	buf[count] = '\0';						\
	return g(buf, base, res);					\
}									\
EXPORT_SYMBOL_GPL(f)

kstrto_from_user(kstrtoull_from_user,	kstrtoull,	unsigned long long);
kstrto_from_user(kstrtoll_from_user,	kstrtoll,	long long);
kstrto_from_user(kstrtoul_from_user,	kstrtoul,	unsigned long);
kstrto_from_user(kstrtol_from_user,	kstrtol,	long);
kstrto_from_user(kstrtouint_from_user,	kstrtouint,	unsigned int);
kstrto_from_user(kstrtoint_from_user,	kstrtoint,	int);
kstrto_from_user(kstrtou16_from_user,	kstrtou16,	u16);
kstrto_from_user(kstrtos16_from_user,	kstrtos16,	s16);
kstrto_from_user(kstrtou8_from_user,	kstrtou8,	u8);
kstrto_from_user(kstrtos8_from_user,	kstrtos8,	s8);
#endif

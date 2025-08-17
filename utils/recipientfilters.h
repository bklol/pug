#pragma once
#include "irecipientfilter.h"
#include "interfaces/interfaces.h"
#include "inetchannel.h"
#include "entity/ccsplayercontroller.h"

class CSingleRecipientFilter : public IRecipientFilter
{
public:
	CSingleRecipientFilter(CPlayerSlot iRecipient, NetChannelBufType_t nBufType = BUF_RELIABLE, bool bInitMessage = false) :
		m_iRecipient(iRecipient), m_nBufType(nBufType), m_bInitMessage(bInitMessage) {}

	~CSingleRecipientFilter() override {}

	NetChannelBufType_t GetNetworkBufType(void) const override { return m_nBufType; }
	bool IsInitMessage(void) const override { return m_bInitMessage; }
	int GetRecipientCount(void) const override { return 1; }
	CPlayerSlot GetRecipientIndex(int slot) const override { return m_iRecipient; }

private:
	CPlayerSlot m_iRecipient;
	NetChannelBufType_t m_nBufType;
	bool m_bInitMessage;
};


class CRecipientFilter : public IRecipientFilter
{
public:
	CRecipientFilter(NetChannelBufType_t nBufType = BUF_RELIABLE, bool bInitMessage = false) : m_nBufType(nBufType), m_bInitMessage(bInitMessage) {}

	~CRecipientFilter() override {}
	
	NetChannelBufType_t GetNetworkBufType(void) const override
	{
		return m_nBufType;
	}
	
	bool IsInitMessage(void) const override
	{
		return m_bInitMessage;
	}

	int GetRecipientCount(void) const override
	{
		return m_Recipients.Count();
	}

	CPlayerSlot GetRecipientIndex(int slot) const override
	{
		if (slot < 0 || slot >= GetRecipientCount())
		{
			return CPlayerSlot(-1);
		}

		return m_Recipients[slot];
	}

	void AddRecipient(CPlayerSlot slot)
	{
		// Don't add if it already exists
		if (m_Recipients.Find(slot) != m_Recipients.InvalidIndex())
		{
			return;
		}

		m_Recipients.AddToTail(slot);
	}

	void AddAllPlayers()
	{
		m_Recipients.RemoveAll();
		for (int i = 0; i <= 64; i++)
		{
			CCSPlayerController *ent = (CCSPlayerController *)GameEntitySystem()->GetEntityInstance(CEntityIndex(i + 1));
			if (ent && ent->IsController())
			{
				AddRecipient(i);
			}
		}
	}

private:
	// Can't copy this unless we explicitly do it!
	CRecipientFilter(CRecipientFilter const &source)
	{
		Assert(0);
	}
	NetChannelBufType_t m_nBufType;
	bool m_bInitMessage;
	CUtlVectorFixed<CPlayerSlot, 64> m_Recipients;
};

class CBroadcastRecipientFilter : public CRecipientFilter
{
public:
	CBroadcastRecipientFilter(void)
	{
		AddAllPlayers();
	}
};
--�Զ�������11436
--����1���������ʯ[3020223]�Ի������BUFF���Զ�ҩ[2011401]
--����2���������ʱɾ��BUFF���Զ�ҩ[2011401]

--����ʯ�Ի�����
function x11436_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3020223 then
	    unit.AddBuff(mapID, instanceID, ownerID, 2011401, ownerID)
	end
end

--������ɺ���
function x11436_OnComplete(mapID, instanceID, questID, ownerID, accepterID)
	unit.CancelBuff(mapID, instanceID, ownerID, 2011401)
end

--Ҫ��Ҫ�������������

--ע��
aux.RegisterQuestEvent(11436, 6, "x11436_OnNPCTalk")
aux.RegisterQuestEvent(11436, 1, "x11436_OnComplete")


--�������ʱ
function x11436_OnCancel(mapID, instanceID, questID, ownerID, enderID)
	unit.CancelBuff(mapID, instanceID, ownerID, 2011401)
end

--ע��
aux.RegisterQuestEvent(11436, 2, "x11436_OnCancel")

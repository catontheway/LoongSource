--���ǿ����10654
--����1����ȡ����10654����BUFF2015501

function x10654_OnAccept(mapID, instanceID, questID, ownerID, accepterID)

	unit.AddBuff(mapID, instanceID, ownerID, 2015501, ownerID)
end

--ע��
aux.RegisterQuestEvent(10654, 0, "x10654_OnAccept")

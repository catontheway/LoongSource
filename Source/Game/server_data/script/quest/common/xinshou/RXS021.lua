

--�������ʱ
function q021_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 8)

end

--ע��
aux.RegisterQuestEvent(21, 1, "q021_OnComplete")


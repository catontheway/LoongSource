--�����¶��11451
--����1����ȡ����ʱ����Ҵ�������ɽͯ�Ӹ���[m03: 2531, 15600, 2475]

--[[�����ȡʱ����
function x11451_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 3017298383, 2531, 15600, 2475)
end

--ע��
aux.RegisterQuestEvent(11451, 0, "x11451_OnAccept")]]

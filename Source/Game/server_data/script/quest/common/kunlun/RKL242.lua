
--15242 �����������NPC������[3070128]�Ի������Ʒ��ı���[2615127]
function x15242_OnNPCTalk(MapID, InstanceID, QuestID, RoleID, Npcid, Npctypeid)
	if Npctypeid == 3070128 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 2615127, 1, -1, 8, 420)
	end
end
--注册
aux.RegisterQuestEvent(15242, 6, "x15242_OnNPCTalk")


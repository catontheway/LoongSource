--RWYMR08��14308�����ξ�
--����1���������[��ҩ��][3060023]����50%����ˢ�¹���[�������][1007109]1��

function q14308_OnInvest(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Rate = math.random(1, 100)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	--ÿ�ε��鶼����һ����Ʒ[ҩ�ۺ���][2614036]
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2614036, 1, 101)
	if Rate < 51 then
	map.MapCreateCreature(MapID, InstanceID, 1007109, x+2, y, z+2)
	end
end

aux.RegisterQuestEvent(14308, 9, "q14308_OnInvest")

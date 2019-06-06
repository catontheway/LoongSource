--���Űħ�� 1008039 ����ս����ˢ�� ��ħС�� 1008040 ���꣺x 1471 y 15188 z 380
--			       ˢ�� ��ħС�� 1008040 ���꣺x 1469 y 15188 z 418
--		           ˢ�� ��ħС�� 1008040 ���꣺x 1434 y 15188 z 417
--			       ˢ�� ��ħС�� 1008040 ���꣺x 1434 y 15188 z 382

function c1008039_On_Enter_Combat(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1471, 15188, 380)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1469, 15188, 418)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1434, 15188, 417)
	map.MapCreateCreature(MapID, InstanceID, 1008040, 1434, 15188, 382)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008039,2,"c1008039_On_Enter_Combat")


--��ɱ�� �Űħ�� 1008039
function c1008039_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local Role = {}
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	for i=1, 6 do
	    if Role[i] and Role[i]~= 4294967295 then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
			if Target_MapID == MapID then
			    role.AddRoleItem(Target_MapID, Target_InstanceID, Role[i], 3305003, 2, -1, 8, 420)
			    if act.GetActIsStart(151) then
				    HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
				end
			end
	    end
	end

	--ˢ�¼�ħ̨
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1531103, x, y, z+4)
	Increase_shili(MapID, InstanceID, RoleID, 1, 3, "jinjieshili")

	map.MapCreateCreature(MapID, InstanceID, 3075221, x, y, z)

end
aux.RegisterCreatureEvent(1008039, 4, "c1008039_OnDie")

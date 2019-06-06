
-- �������ս������/ʾ���ű�
function c1005420_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 20064)
	    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end
aux.RegisterCreatureEvent(1005420, 2, "c1005420_OnEnterCombat")


-- ��ɱ �������� 1005420�� ˢ�� �л걮 3030402�����꣺x 272 y 5270 z 172
function c1005420_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 20066)
	map.MapCreateCreature(MapID, InstanceID, 3030402, 272, 5270, 172)

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
	map.MapCreateCreature(MapID, InstanceID, 1531101, x, y, z+4)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	map.MapCreateCreature(MapID, InstanceID, 3075221, x, y, z)

end
aux.RegisterCreatureEvent(1005420, 4, "c1005420_OnDie")

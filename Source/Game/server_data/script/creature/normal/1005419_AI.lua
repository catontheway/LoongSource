--1005419	ħԳ��(����ħ��) /ʾ���ű�
--����ս�����ٻ� {1005411 ��������[��ǰ]} {1005412 ��������[��ǰ]}
function c1005419_On_Enter_Combat(MapID, InstanceID, CreatureID)
	-- �������ս������
	cre.MonsterSay(MapID, InstanceID, CreatureID, 20059)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	map.MapCreateCreature(MapID, InstanceID, 1005411, x-5, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1005412, x+5, y, z)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1005419,2,"c1005419_On_Enter_Combat")

--������������
function c1005419_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 20061)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1005419, 4, "c1005419_OnDie")




